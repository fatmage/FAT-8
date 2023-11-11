#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "chip.h"
#include "platform.h"


int main(int argc, char **argv) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }

    init_platform(argv[1]);
    run_chip();


    return EXIT_SUCCESS;
} 