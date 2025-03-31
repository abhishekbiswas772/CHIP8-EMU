#include "chip8.h"
#include <memory.h>
#include <assert.h>

const char chip8_default_character_set[] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xf0, 0x10, 0xf0, 0x80, 0xf0,
    0xf0, 0x10, 0xf0, 0x10, 0xf0,
    0x90, 0x90, 0xf0, 0x10, 0x10,
    0xf0, 0x80, 0xf0, 0x10, 0xf0,
    0xf0, 0x80, 0xf0, 0x90, 0xf0,
    0xf0, 0x10, 0x20, 0x40, 0x40,
    0xf0, 0x90, 0xf0, 0x90, 0xf0,
    0xf0, 0x90, 0xf0, 0x10, 0xf0,
    0xf0, 0x90, 0xf0, 0x90, 0x90,
    0xe0, 0x90, 0xe0, 0x90, 0xe0,
    0xf0, 0x80, 0x80, 0x80, 0xf0,
    0xe0, 0x90, 0x90, 0x90, 0xe0,
    0xf0, 0x80, 0xf0, 0x80, 0xf0,
    0xf0, 0x80, 0xf0, 0x80, 0x80
};

void chip8_init(struct chip8 *chip8)
{
    memset(chip8, 0, sizeof(struct chip8));
    memcpy(&chip8->memory.memory, chip8_default_character_set, sizeof(chip8_default_character_set));
}

static void chip8_exec_extended(struct chip8* chip8, unsigned short opcode){

    unsigned short nnn = opcode & 0x0fff;
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char kk = (opcode & 0x00ff);

    switch (opcode & 0xf000){
        case 0x1000:
            chip8->registers.PC = nnn;
            break;
        case 0x2000:
            chip8_stack_push(chip8, chip8->registers.PC);
            chip8->registers.PC = nnn;
            break;
        case 0x3000:
            if(chip8->registers.V[x] == kk){
                chip8->registers.PC += 2;
            }
            break;
        case 0x4000:
            if(chip8->registers.V[x] != kk){
                chip8->registers.PC += 2;
            }
            break;
        case 0x5000:
            if(chip8->registers.V[x] == chip8->registers.V[y]){
                chip8->registers.PC += 2;
            }
            break        
    }
}

void chip8_exec(struct chip8* chip8, unsigned short opcode){
    switch(opcode){
        case 0x00E0:
            chip8_screen_clear(&chip8->screen);
            break;
        case 0x00EE:
            chip8->registers.PC = chip8_stack_pop(chip8);
            break;
        default:
            chip8_exec_extended(chip8, opcode);
    }
}

void chip8_load(struct chip8* chip8, const char* buff, size_t size){
    assert(size + CHIP8_CHARACTER_SET_LOADED_ADDRESS < CHIP8_MEMORY_SIZE);
    memcpy(&chip8->memory.memory[CHIP8_CHARACTER_SET_LOADED_ADDRESS], buff, size);
    chip8->registers.PC = CHIP8_CHARACTER_SET_LOADED_ADDRESS;
}