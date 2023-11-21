#include "chip.h"

static fat8_t fat8;
keypad_t fat8_keypad;
static void (*op_table[0xF]) ();
static void (*op_table0[0xF]) ();
static void (*op_table8[0xF]) ();
static void (*op_tableE[0xF]) ();
static void (*op_tableF[0x65]) ();


void fat8_init() {
    memset(&fat8, 0, sizeof(fat8_t));
    memset(&fat8_keypad, 0, sizeof(keypad_t));

    fat8.PC = INSTRUCTION_START;
    fat8.RSP = -1;

    // initialise fonts
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

    // initialise instruction tables

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
            op_table0[i] = &op_noop;
            op_table8[i] = &op_noop;
            op_tableE[i] = &op_noop;
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
			op_tableF[i] = &op_noop;
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


    // initialise rng
    srand(time(NULL));
}


void fat8_operation() {

    // fetch
    fat8.current_opcode = (fat8.memory[fat8.PC] << 8) | fat8.memory[fat8.PC+1];
    fat8.PC += 2;

    // decode & execute
    (*op_table[(fat8.current_opcode & 0xF000) >> 12])();
}

void fat8_timers() {
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
        if (fread(&fat8.memory[i], 1, 1, rom_file) == 0) 
            break;

    if (ferror(rom_file)) {
        res = 1;
    }

    fclose(rom_file);
    return res;
}

int fat8_export_framebuffer(pixel_t *dst) {
    if (dst == NULL) {
        // error
        return 1;
    }

    for (int i = 0; i < FRAME_BUFFER_SIZE; i++) {
        dst[i] = fat8.frame_buffer[i];
    }

    return 0;
}

// operations

void help_table0() { (*op_table0[fat8.current_opcode & 0x000F])(); }

void help_table8() { (*op_table8[fat8.current_opcode & 0x000F])(); }

void help_tableE() { (*op_tableE[fat8.current_opcode & 0x000F])(); }

void help_tableF() { (*op_tableF[fat8.current_opcode & 0x00FF])(); }

void op_noop() { return; }

void op_0nnn() { // SYS addr
/*
 * According to Cowgod's CHIP-8 Technical Reference:
 * Jump to a machine code routine at nnn.
 * This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters.
 * 
 * Here equivalent to CALL addr
 */
    op_2nnn();
}

void op_00E0() { // CLS
    for (int i = 0; i < FRAME_BUFFER_SIZE; i++)
        fat8.frame_buffer[i] = PIXEL_OFF;
}

void op_00EE() { // RET
    fat8.PC = fat8.stack[fat8.RSP];
    fat8.RSP -= 1;
}

void op_1nnn() { // JP addr
    uint16_t jump_target = fat8.current_opcode & 0x0FFF;
    fat8.PC = jump_target;
}

void op_2nnn() { // CALL addr
    uint16_t jump_target = fat8.current_opcode & 0x0FFF;
    fat8.RSP++;
    fat8.stack[fat8.RSP] = fat8.PC;
    fat8.PC = jump_target;
}

void op_3xkk() { // SE Vx, byte
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t val = fat8.current_opcode & 0x00FF;
    if (fat8.V[Vx] == val) {
        fat8.PC += 2;
    }
}

void op_4xkk() { // SNE Vx, byte
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t val = fat8.current_opcode & 0x00FF;
    if (fat8.V[Vx] != val) {
        fat8.PC += 2;
    }
}

void op_5xy0() { // SE Vx, Vy
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t Vy = (fat8.current_opcode & 0x00F0) >> 4;
    if (fat8.V[Vx] == fat8.V[Vy]) {
        fat8.PC += 2;
    }
}

void op_6xkk() { // LD Vx, byte
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t val = fat8.current_opcode & 0x00FF;
    fat8.V[Vx] = val;
}

void op_7xkk() { // ADD Vx, byte
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t val = fat8.current_opcode & 0x00FF;
    fat8.V[Vx] += val;
}

void op_8xy0() { // LD Vx, Vy
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t Vy = (fat8.current_opcode & 0x00F0) >> 4;
    fat8.V[Vx] = fat8.V[Vy];
}

void op_8xy1() { // OR Vx, Vy
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t Vy = (fat8.current_opcode & 0x00F0) >> 4;
    fat8.V[Vx] |= fat8.V[Vy];
}

void op_8xy2() { // AND Vx, Vy
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t Vy = (fat8.current_opcode & 0x00F0) >> 4;
    fat8.V[Vx] &= fat8.V[Vy];
}

void op_8xy3() { // XOR Vx, Vy
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t Vy = (fat8.current_opcode & 0x00F0) >> 4;
    fat8.V[Vx] ^= fat8.V[Vy];
}

void op_8xy4() { // ADD Vx, Vy
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t Vy = (fat8.current_opcode & 0x00F0) >> 4;
    uint16_t result = fat8.V[Vx] + fat8.V[Vy];
    fat8.V[Vx] = result & 0x00FF;
    fat8.V[0xF] = result > 255;
}

void op_8xy5() { // SUB Vx, Vy
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t Vy = (fat8.current_opcode & 0x00F0) >> 4;
    uint8_t flag = fat8.V[Vx] >= fat8.V[Vy];
    fat8.V[Vx] -= fat8.V[Vy];
    fat8.V[0xF] = flag;
}

void op_8xy6() { // SHR Vx {, Vy}
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t Vy = (fat8.current_opcode & 0x00F0) >> 4;
    uint8_t flag = fat8.V[Vy] & 0x01;
    fat8.V[Vx] = fat8.V[Vy] >> 1;
    fat8.V[0xF] = flag;
}

void op_8xy7() { // SUBN Vx, Vy
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t Vy = (fat8.current_opcode & 0x00F0) >> 4;
    uint8_t flag = fat8.V[Vx] <= fat8.V[Vy];
    fat8.V[Vx] = fat8.V[Vy] - fat8.V[Vx];
    fat8.V[0xF] = flag;
}

void op_8xyE() { // SHL Vx {, Vy}
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t Vy = (fat8.current_opcode & 0x00F0) >> 4;
    uint8_t flag = (fat8.V[Vy] & 0x80) >> 7;
    fat8.V[Vx] = fat8.V[Vy] << 1;
    fat8.V[0xF] = flag;
}

void op_9xy0() { // SNE Vx, Vy
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t Vy = (fat8.current_opcode & 0x00F0) >> 4;
    if (fat8.V[Vx] != fat8.V[Vy]) {
        fat8.PC += 2;
    }
}

void op_Annn() { // LD I, addr
    uint16_t val = fat8.current_opcode & 0x0FFF;
    fat8.IR = val;
}

void op_Bnnn() { // JP V0, addr
    uint16_t addr = fat8.current_opcode & 0x0FFF;
    fat8.PC = fat8.V[0] + addr;
}

void op_Cxkk() { // RND Vx, byte
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t val = fat8.current_opcode & 0x00FF;
    uint16_t random_byte = rand() % 256;
    fat8.V[Vx] = val & random_byte;
}

void op_Dxyn() { // DRW Vx, Vy, nibble
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint8_t Vy = (fat8.current_opcode & 0x00F0) >> 4;
    uint16_t byte_num = fat8.current_opcode & 0x000F;


    pixel_t sprite[byte_num][8];
    for (int i = 0; i < byte_num; i++) {
        uint8_t sprite_byte = fat8.memory[fat8.IR + i];
        for (int j = 0; j < 8 ; j++) {
            sprite[i][j] = (sprite_byte & (0x80 >> j)) ? PIXEL_ON : PIXEL_OFF;
        }
    }

    uint16_t x = fat8.V[Vx] % FRAME_BUFFER_WIDTH;
    uint16_t y = fat8.V[Vy] % FRAME_BUFFER_HEIGHT;
    fat8.V[0xF] = 0;
    for (int i = 0; i < byte_num; i++) {
        for (int j = 0; j < 8; j++) {
            
            if (fat8.frame_buffer[IND((y + i) % 32, (x + j) % 64)] && !sprite[i][j]) {
                fat8.V[0xF] = 1;
            }
            fat8.frame_buffer[IND((y + i) % 32, (x + j) % 64)] ^= sprite[i][j];
        }
    }
}

void op_Ex9E() { // SKP Vx
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    if (fat8_keypad.key[fat8.V[Vx]]) {
        fat8.PC += 2;
    }
}

void op_ExA1() { // SKNP Vx
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    if (!fat8_keypad.key[fat8.V[Vx]]) {
        fat8.PC += 2;
    }
}

void op_Fx07() { // LD Vx, DT
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    fat8.V[Vx] = fat8.delay_timer;
}

void op_Fx0A() { // LD Vx, K
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    for (int i = 0; i <= 0xF; i++) {
        if (fat8_keypad.key[i]) {
            fat8.V[Vx] = i;
            return;
        }
    }
    fat8.PC -= 2;
}

void op_Fx15() { // LD DT, Vx
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    fat8.delay_timer = fat8.V[Vx];
}

void op_Fx18() { // LD ST, Vx
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    fat8.sound_timer = fat8.V[Vx];
}

void op_Fx1E() { // ADD I, Vx
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    fat8.IR = fat8.IR + fat8.V[Vx];
}

void op_Fx29() { // LD F, Vx   
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    fat8.IR = FONTSET_START + (fat8.V[Vx] * 5);
}

void op_Fx33() { // LD B, Vx
    uint8_t Vx = (fat8.current_opcode & 0x0F00) >> 8;
    uint16_t val = fat8.V[Vx];
    fat8.memory[fat8.IR]     = (val / 100) % 10;
    fat8.memory[fat8.IR + 1] = (val / 10)  % 10;
    fat8.memory[fat8.IR + 2] =  val        % 10;
}

void op_Fx55() { // LD [I], Vx
    uint16_t x = (fat8.current_opcode & 0x0F00) >> 8;
    for (int i = 0; i <= x; i++)
        fat8.memory[fat8.IR + i] = fat8.V[i];
}

void op_Fx65() { // LD Vx, [I]
    uint16_t x = (fat8.current_opcode & 0x0F00) >> 8;
    for (int i = 0; i <= x; i++)
        fat8.V[i] = fat8.memory[fat8.IR + i];
}








