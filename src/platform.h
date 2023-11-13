#ifndef PLATFORM_H
#define PLATFORM_H

#include <ncurses.h>
#include "chip.h"
#include <sys/time.h>
#include <time.h>


const uint64_t frame_time = 1000000/60;



int init_platform(const char * rom_path);
int draw_screen();
int get_keys();
int run_chip();




#endif