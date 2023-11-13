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

    int keys[16];
    memset(keys, 0, 16*sizeof(int));

    int pressed_key;

    while ((pressed_key = getch()) != ERR) {
        switch (pressed_key) {
            case '1':
                keys[0x1] = pressed_key;
                break; 
            case '2':
                keys[0x2] = pressed_key;
                break; 
            case '3':
                keys[0x3] = pressed_key;
                break; 
            case '4':
                keys[0xC] = pressed_key;
                break; 
            case 'q':
                keys[0x4] = pressed_key;
                break; 
            case 'w':
                keys[0x5] = pressed_key;
                break; 
            case 'e':
                keys[0x6] = pressed_key;
                break; 
            case 'r':
                keys[0xD] = pressed_key;
                break; 
            case 'a':
                keys[0x7] = pressed_key;
                break; 
            case 's':
                keys[0x8] = pressed_key;
                break; 
            case 'd':
                keys[0x9] = pressed_key;
                break; 
            case 'f':
                keys[0xE] = pressed_key;
                break; 
            case 'z':
                keys[0xA] = pressed_key;
                break; 
            case 'x':
                keys[0x0] = pressed_key;
                break; 
            case 'c':
                keys[0xB] = pressed_key;
                break; 
            case 'v':
                keys[0xF] = pressed_key;
                break; 
            default:
                break;
        }
    }

    for (int i = 0x0; i <= 0xF; i++) {
            fat8_keypad.key[i] = keys[i];
    }

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

    struct timeval prev;
    gettimeofday(&prev, NULL);
    uint64_t time_elapsed = 0;

    for (;;) {
        fat8_export_framebuffer(screen);
        draw_screen();
        wrefresh(stdscr);
        wrefresh(chip_window);


        struct timeval curr;
        gettimeofday(&curr, NULL);

        uint64_t time_delta = curr.tv_sec * 1000000;
        time_delta += curr.tv_usec;
        time_delta -= prev.tv_sec * 1000000;
        time_delta -= prev.tv_usec;

        time_elapsed += time_delta;

        if (time_elapsed > frame_time) {
            time_elapsed -= frame_time;
            get_keys();
            fat8_cycle();
        }

        prev = curr;
    }

    return 0;
}


