#ifndef CHIP8_H
#define CHIP8_H

#include "chip8memory.h"
#include "config.h"
#include "chip8register.h"
#include "chip8stack.h"
#include "chip8keyboad.h"
#include "chip8screen.h"

struct chip8
{
    struct chip8_memory memory;
    struct chip8_registers registers;
    struct chip8_stack stack;
    struct chip8_keyboard keyboard;
    struct chip8_screen screen;
};


void chip8_init(struct chip8* chip);


#endif