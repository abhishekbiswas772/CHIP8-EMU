#ifndef CHIP8_H
#define CHIP8_H

#include "chip8memory.h"
#include "config.h"
#include "chip8register.h"
#include "chip8stack.h"
#include "chip8keyboard.h"
#include "chip8screen.h"
#include <stddef.h>

struct chip8
{
    struct chip8_memory memory;
    struct chip8_registers registers;
    struct chip8_stack stack;
    struct chip8_keyboard keyboard;
    struct chip8_screen screen;
};


void chip8_init(struct chip8* chip);
void chip8_exec(struct chip8* chip8, unsigned short opcode);
void chip8_load(struct chip8* chip8, const char* buff, size_t size);


#endif