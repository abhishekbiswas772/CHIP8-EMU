#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "SDL.h"

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} sdl_t;


typedef struct {
    uint32_t window_width;
    uint32_t window_height;
    uint32_t fg_color;
    uint32_t bg_color;
    uint32_t scale_factor;
} config_t;


typedef enum {
    QUIT,
    RUNNING ,
    PAUSED,
} emulator_state_t;

typedef struct {
    uint16_t opcode;
    uint16_t NNN;
    uint8_t NN;
    uint8_t N;
    uint8_t Y;
    uint8_t X;
} instructions_t;

typedef struct {
    emulator_state_t state;
    uint8_t ram[4096];
    bool display[64 *32];
    uint16_t stack[16];
    uint16_t *stack_ptr;
    uint8_t V[16];
    uint16_t I;
    uint8_t delay_timer;
    uint8_t sound_timer;
    bool keypad[16];
    const char *rom_name;
    uint16_t PC;
    instructions_t inst;
} chip8_t;

bool init_sdl(sdl_t* sdl, const config_t config){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)){
        SDL_Log("Could not initalize the subsystem for SDL! %s\n", SDL_GetError());
        return false;
    }
    sdl->window = SDL_CreateWindow("Chip 8 Emulator",  
                                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.window_width * config.scale_factor, 
                                    config.window_height * config.scale_factor, 0);
    if(!sdl->window){
        SDL_Log("Could not create SDL Window %s\n", SDL_GetError());
        return false;
    }
    sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
    if(!sdl->renderer){
        SDL_Log("Could not create SDL Renderer %s\n", SDL_GetError());
        return false;
    }
    return true;
}


bool set_config_from_args(config_t *config, const int argc, char** argv){
    *config = (config_t){
        .window_height = 64,
        .window_width = 32,
        .fg_color = 0xFFFFFFFF,
        .bg_color = 0x00000000,
        .scale_factor = 10
    };
    for(int i = 0; i < argc; i++){
        (void)argv[i];
    }
    return true;
}


void final_cleanup(const sdl_t sdl){
    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window);
    SDL_Quit();
}


void clear_screen(const sdl_t sdl, const config_t config){
    const uint8_t r = (config.bg_color >> 24) & 0xFF;
    const uint8_t g = (config.bg_color >> 16) & 0xFF;
    const uint8_t b = (config.bg_color >> 8) & 0xFF;
    const uint8_t a = (config.bg_color >> 0) & 0xFF;
    SDL_SetRenderDrawColor(sdl.renderer, r, g, b, a);
    SDL_RenderClear(sdl.renderer);
}


void update_screen(const sdl_t sdl){
    SDL_RenderPresent(sdl.renderer);
}


void handle_input(chip8_t *chip8){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                chip8->state = QUIT;
                return;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        chip8->state = QUIT;
                        return;
                    case SDLK_SPACE:
                        if(chip8->state == RUNNING){
                            chip8->state = PAUSED;
                            puts("===== PAUSED =====");
                        } else {
                            chip8->state = RUNNING;
                            puts("===== RUNNING =====");
                        }
                        return;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                break;
            default:
                break;
        }
    }
}

bool init_chip8(chip8_t* chip8, const char rom_name[]) {
    if (chip8 == NULL) {
        return false;
    }
    const uint32_t entry_point = 0x200;
    const uint8_t font[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,   // 0   
        0x20, 0x60, 0x20, 0x20, 0x70,   // 1  
        0xF0, 0x10, 0xF0, 0x80, 0xF0,   // 2 
        0xF0, 0x10, 0xF0, 0x10, 0xF0,   // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,   // 4    
        0xF0, 0x80, 0xF0, 0x10, 0xF0,   // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,   // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,   // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,   // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,   // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,   // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,   // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,   // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,   // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,   // E
        0xF0, 0x80, 0xF0, 0x80, 0x80,   // F
    };
    memcpy(&chip8->ram[0], font, sizeof(font));
    FILE *rom = fopen(rom_name, "rb");
    if (!rom){
        SDL_Log("Rom File %s is invalid and doesnot exits\n", rom_name);
        return false;
    }
    fseek(rom, 0, SEEK_END);
    const size_t rom_size = ftell(rom);
    const size_t max_size = sizeof chip8->ram - entry_point;
    rewind(rom);
    if(rom_size > max_size){
        SDL_Log("Rom File %s is to big! Rom size %zu, Max size allow %zu\n", rom_name, rom_size, max_size);
        return false;
    } 
    if(fread(&chip8->ram[entry_point], rom_size, 1, rom) !=  1){
        SDL_Log("Could read Rom File %s into chip8 memory", rom_name);
        return false;
    }
    fclose(rom);
    chip8->state = RUNNING;
    chip8->PC = entry_point;
    chip8->rom_name = rom_name;
    chip8->stack_ptr = &chip8->stack[0];
    return true;
}


void emulate_instruction(chip8_t *chip8){
    chip8->inst.opcode = (chip8->ram[chip8->PC] << 8) | chip8->ram[chip8->PC + 1];
    chip8->PC += 2;
    chip8->inst.NNN = chip8->inst.opcode & 0x0FFF;
    chip8->inst.NN = chip8->inst.opcode & 0x0FF;
    chip8->inst.N = chip8->inst.opcode & 0x0F;
    chip8->inst.X = (chip8->inst.opcode >> 8) & 0x0F;
    chip8->inst.Y = (chip8->inst.opcode >> 4) & 0x0F;

    switch ((chip8->inst.opcode >> 12) & 0x0F)
    {
        case 0x00:
            if(chip8->inst.NN == 0xE0){
                memset(&chip8->display[0], false, sizeof chip8->display);
            }else if (chip8->inst.NN == 0xEE){
                chip8->PC = *--chip8->stack_ptr; 
            }else{
                puts("unimplemented code,..");
            }
            break;
        case 0x02:
            *chip8->stack_ptr++ = chip8->PC;
            chip8->PC = chip8->inst.NNN;
            break;       
        default:
            puts("Unimplemented or invalid part");
            break;
    }

}

int main(int argc, char** argv){

    if(argc < 2){
        fprintf(stderr, "Usage: %s <rom_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    config_t config = {0};
    if(!set_config_from_args(&config, argc, argv)) exit(EXIT_FAILURE);
    sdl_t sdl = {0};
    if(!init_sdl(&sdl, config)) exit(EXIT_FAILURE);
    chip8_t chip8 = {0};
    const char *rom_name = argv[1];
    if(!init_chip8(&chip8, rom_name)) exit(EXIT_FAILURE);
    clear_screen(sdl, config);
    while(chip8.state != QUIT){
        handle_input(&chip8);
        if (chip8.state == PAUSED) continue;
        emulate_instruction(&chip8);
        SDL_Delay(16);
        update_screen(sdl);

    }
    final_cleanup(sdl);
    exit(EXIT_SUCCESS);
    return 0;
}