#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "chip.h"







int main(int argc, char **argv) {


    fat8_init();
    fat8_cycle();



    return EXIT_SUCCESS;
} 