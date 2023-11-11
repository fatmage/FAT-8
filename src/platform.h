#ifndef PLATFORM_H
#define PLATFORM_H

#include <ncurses.h>
#include "chip.h"




int init_platform(const char * rom_path);
int draw_screen();
int get_keys();
int run_chip();




#endif