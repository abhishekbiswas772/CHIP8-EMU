#ifndef CHIP8REGISTER_H
#define CHIP8REGISTER_H

#include "config.h"

struct chip8_registers 
{
    unsigned char V[CHIP8_TOTAL_DATA_REGISTERS];
    unsigned char I;
    unsigned char sound_timer;
    unsigned char delay_timer;
    unsigned char PC;
    unsigned char SP;
};


#endif