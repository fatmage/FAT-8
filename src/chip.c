#include "chip.h"

static fat8_t fat8;
static void (*op_table[0xF]) ();
static void (*op_table0[0xF]) ();
static void (*op_table8[0xF]) ();
static void (*op_tableE[0xF]) ();
static void (*op_tableF[0x65]) ();


void fat8_init() {
    memset(&fat8, 0, sizeof(fat8_t));
    fat8.PC = INSTRUCTION_START;

    // init fontów

    byte_t fontset[FONTSET_SIZE] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for (int i = 0; i < FONTSET_SIZE; i++)
        fat8.memory[FONTSET_START + i] = fontset[i];

    // init instrukcji

    	op_table[0x0] = &help_table0;
		op_table[0x1] = &op_1nnn;
		op_table[0x2] = &op_2nnn;
		op_table[0x3] = &op_3xkk;
		op_table[0x4] = &op_4xkk;
		op_table[0x5] = &op_5xy0;
		op_table[0x6] = &op_6xkk;
		op_table[0x7] = &op_7xkk;
		op_table[0x8] = &help_table8;
		op_table[0x9] = &op_9xy0;
		op_table[0xA] = &op_Annn;
		op_table[0xB] = &op_Bnnn;
		op_table[0xC] = &op_Cxkk;
		op_table[0xD] = &op_Dxyn;
		op_table[0xE] = &help_tableE;
		op_table[0xF] = &help_tableF;

        for (int i = 0; i < 16; i++) {
            op_table0[i] = &op_NULL;
            op_table8[i] = &op_NULL;
            op_tableE[i] = &op_NULL;
        }

        op_table0[0x0]  = &op_00E0;
		op_table0[0xE] = &op_00EE;

        op_table8[0x0] = &op_8xy0;
		op_table8[0x1] = &op_8xy1;
		op_table8[0x2] = &op_8xy2;
		op_table8[0x3] = &op_8xy3;
		op_table8[0x4] = &op_8xy4;
		op_table8[0x5] = &op_8xy5;
		op_table8[0x6] = &op_8xy6;
		op_table8[0x7] = &op_8xy7;
		op_table8[0xE] = &op_8xyE;

        op_tableE[0x1] = &op_ExA1;
		op_tableE[0xE] = &op_Ex9E;

		for (int i = 0; i <= 0x65; i++) {
			op_tableF[i] = &op_NULL;
		}

		op_tableF[0x07] = &op_Fx07;
		op_tableF[0x0A] = &op_Fx0A;
		op_tableF[0x15] = &op_Fx15;
		op_tableF[0x18] = &op_Fx18;
		op_tableF[0x1E] = &op_Fx1E;
		op_tableF[0x29] = &op_Fx29;
		op_tableF[0x33] = &op_Fx33;
		op_tableF[0x55] = &op_Fx55;
		op_tableF[0x65] = &op_Fx65;


    // init rng
    srand(time(NULL));
}


void fat8_cycle() {

    // fetch
    fat8.current_opcode = (fat8.memory[fat8.PC] << 8) | fat8.memory[fat8.PC+1];
    fat8.PC += 2;

    // decode
    (*op_table[(fat8.current_opcode & 0xF000) >> 12])();
    // execute

    // timers
    if (fat8.delay_timer > 0)
        fat8.delay_timer--;
    if (fat8.sound_timer > 0)
        fat8.sound_timer--;
}


int fat8_load_ROM(const char * ROM_path) {

    int res = 0;
    FILE * rom_file;

    if ((rom_file = fopen(ROM_path, "r")) == NULL) 
        return 1;

    for (int i = INSTRUCTION_START; i < 4096; i++)
        if (fread(fat8.memory[i], 1, 1, rom_file) == 0) 
            break;

    if (ferror(rom_file)) {
        res = 1;
    }

    fclose(rom_file);
    return res;
}

// operations

void help_table0() { (*op_table0[fat8.current_opcode & 0x000F])(); }

void help_table8() { (*op_table8[fat8.current_opcode & 0x000F])(); }

void help_tableE() { (*op_tableE[fat8.current_opcode & 0x000F])(); }

void help_tableF() { (*op_tableF[fat8.current_opcode & 0x00FF])(); }

void op_NULL() { return; }

void op_0nnn() {

}

void op_00E0() {

}

void op_00EE() {

}

void op_1nnn() {

}

void op_2nnn() {

}

void op_3xkk() {

}

void op_4xkk() {

}

void op_5xy0() {

}

void op_6xkk() {

}

void op_7xkk() {

}

void op_8xy0() {

}

void op_8xy1() {

}

void op_8xy2() {

}

void op_8xy3() {

}

void op_8xy4() {

}

void op_8xy5() {

}

void op_8xy6() {

}

void op_8xy7() {

}

void op_8xyE() {

}

void op_9xy0() {

}

void op_Annn() {

}

void op_Bnnn() {

}

void op_Cxkk() {

}

void op_Dxyn() {

}

void op_Ex9E() {

}

void op_ExA1() {

}

void op_Fx07() {

}
void op_Fx0A() {

}

void op_Fx15() {

}

void op_Fx18() {

}

void op_Fx1E() {

}

void op_Fx29() {

}

void op_Fx33() {

}

void op_Fx55() {

}

void op_Fx65() {

}








