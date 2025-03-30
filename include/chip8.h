#ifndef CHIP8_H
#define CHIP8_h

#include "chip8memory.h"
#include "config.h"
#include "chip8register.h"

struct chip8
{
    struct chip8_memory memory;
    struct chip8_registers registers;

};


#endif