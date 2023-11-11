#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define FONTSET_SIZE 80
#define FONTSET_START 0x000
#define FONTSET_END 0x050
#define INSTRUCTION_START 0x200
#define CALL_STACK_START 0xEA0
#define CALL_STACK_END 0xEFF
#define DISPLAY_START 0xF00 
#define RESERVED_END 0x1FF

const int SCREEN_BUFFER_SIZE = 32 * 64;

#define USER_SPACE_START INSTRUCTION_START
#define USER_SPACE_END   4096
#define MAX_STACK_DEPTH  16


typedef uint8_t byte_t;
typedef uint8_t pixel_t;

typedef struct fat8 {
    pixel_t screen_buffer[SCREEN_BUFFER_SIZE];
    byte_t memory[4096];
    // special registers
    uint16_t PC;
    uint16_t IR;
    // stack
    uint16_t stack[16];
    uint8_t RSP;
    // regular registers
    uint8_t V[16];
    // timers
    uint8_t delay_timer;
    uint8_t sound_timer;

    // helper variables
    uint16_t current_opcode;
} fat8_t;


void fat8_init();
void fat8_cycle();
int fat8_load_ROM(const char * ROM_path);

// operations

void help_table0();
void help_table8();
void help_tableE();
void help_tableF();

void op_noop();

void op_0nnn();
void op_00E0();
void op_00EE();
void op_1nnn();
void op_2nnn();
void op_3xkk();
void op_4xkk();
void op_5xy0();
void op_6xkk();
void op_7xkk();
void op_8xy0();
void op_8xy1();
void op_8xy2();
void op_8xy3();
void op_8xy4();
void op_8xy5();
void op_8xy6();
void op_8xy7();
void op_8xyE();
void op_9xy0();
void op_Annn();
void op_Bnnn();
void op_Cxkk();
void op_Dxyn();
void op_Ex9E();
void op_ExA1();
void op_Fx07();
void op_Fx0A();
void op_Fx15();
void op_Fx18();
void op_Fx1E();
void op_Fx29();
void op_Fx33();
void op_Fx55();
void op_Fx65();

// Super Chip NOT IMPLEMENTED YET
void op_00Cn();
void op_00FB();
void op_00FC();
void op_00FD();
void op_00FE();
void op_00FF();
void op_Dxy0();
void op_Fx30();
void op_Fx75();
void op_Fx85();


#endif

