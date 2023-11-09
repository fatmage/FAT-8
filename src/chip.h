#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define RESERVED_END 0x1FF
#define INSTRUCTION_START 0x200
#define STORAGE_START 0x050
#define STORAGE_END 0x0A0

typedef uint8_t byte_t;
typedef uint8_t pixel_t;

typedef struct fat8 {
    pixel_t screen_buffer[32 * 64];
    byte_t memory[4096];
    // special registers
    uint16_t PC;
    uint16_t IR;
    // stack
    uint16_t stack[16];
    uint8_t RSP;
    // regular registers
    uint8_t V0;
    uint8_t V1;
    uint8_t V2;
    uint8_t V3;
    uint8_t V4;
    uint8_t V5;
    uint8_t V6;
    uint8_t V7;
    uint8_t V8;
    uint8_t V9;
    uint8_t VA;
    uint8_t VB;
    uint8_t VC;
    uint8_t VD;
    uint8_t VE;
    uint8_t VF;
    // timers
    uint8_t delay_timer;
    uint8_t sound_timer;
} fat8_t;


void fat8_init();

void fat8_cycle();

#endif

