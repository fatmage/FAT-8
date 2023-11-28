#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "chip.h"
#include "platform.h"


void print_usage() {
    printf(
"Usage: \e[1mfat8\e[0m \e[4mrom_path\e[0m [\e[4mSPEED_MULTIPLIER\e[0m]\n"
);


}


int main(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        print_usage();
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