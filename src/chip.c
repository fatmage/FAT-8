#include "chip.h"

static fat8_t fat8;

void fat8_init() {
    memset(&fat8, 0, sizeof(fat8_t));
    fat8.PC = INSTRUCTION_START;
}


void fat8_cycle() {

    // fetch
    uint16_t op = (fat8.memory[fat8.PC] << 8) | fat8.memory[fat8.PC+1];
    fat8.PC += 2;

    // decode
    // execute

    // timers
    if (fat8.delay_timer > 0)
        fat8.delay_timer--;
    if (fat8.sound_timer > 0)
        fat8.sound_timer--;
}

