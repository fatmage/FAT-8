#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "chip.h"
#include "platform.h"


int main(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        return EXIT_FAILURE;
    }

    if (argc == 3) {
        init_platform(argv[1], atoi(argv[2]));
    } else {
        init_platform(argv[1], 4);
    }

    run_chip();


    return EXIT_SUCCESS;
} 