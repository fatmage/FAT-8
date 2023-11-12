#include "platform.h"
#include "chip.h"


extern keypad_t fat8_keypad;
static WINDOW* chip_window;
static pixel_t screen[64 * 32];


int init_platform(const char * rom_path) {

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);
    start_color();
    init_pair(PIXEL_ON,   COLOR_WHITE, COLOR_WHITE);
    init_pair(PIXEL_OFF,  COLOR_BLACK, COLOR_BLACK);
    curs_set(0);

    int chip_window_start_y = (LINES - 32) / 2;
    int chip_window_start_x = (COLS  - 128) / 2;

    chip_window = newwin(32, 128, chip_window_start_y, chip_window_start_x);

    fat8_init();
    fat8_load_ROM(rom_path);

    return 0;
}


int get_keys() {

    return 0;
}



int draw_screen() {
    wmove(chip_window, 0, 0);
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
                waddch(chip_window, ' ' | COLOR_PAIR(screen[IND(y, x)]));
                waddch(chip_window, ' ' | COLOR_PAIR(screen[IND(y, x)]));
        }
        wmove(chip_window, y+1, 0);
    }
    
    return 0;
}

int run_chip() {

    for (;;) {
        fat8_export_framebuffer(screen);
        draw_screen();
        wrefresh(stdscr);
        wrefresh(chip_window);

        get_keys();

        fat8_cycle();
    }

    return 0;
}


