#include "platform.h"
#include "chip.h"


extern keypad_t fat8_keypad;
#ifdef NCURSES
static WINDOW* chip_window;
#endif
static pixel_t screen[64 * 32];

const uint64_t timer_time = 1000000/60;
uint64_t frame_time = 1000000/60;

#define KEYBOARD_TIMEOUT_CYCLES 10


int init_platform(const char * rom_path, uint64_t scale) {

    frame_time /= scale;

#ifdef NCURSES
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
#endif

    fat8_init();
    fat8_load_ROM(rom_path);

    return 0;
}


int clear_keys() {
    for (int i = 0x0; i <= 0xF; i++) {

        if (fat8_keypad.time_left[i] > 0) {
            fat8_keypad.time_left[i] -= 1;
            if (fat8_keypad.time_left[i] == 0)
                fat8_keypad.key[i] = 0;
        }
    }

    return 0;
}

#define set_key(kboard, kpad)                                  \
    case kboard:                                               \
        fat8_keypad.key[kpad] = 1;                             \
        fat8_keypad.time_left[kpad] = KEYBOARD_TIMEOUT_CYCLES; \
        break;    

int get_keys() {

#ifdef NCURSES
    int pressed_key;

    while ((pressed_key = getch()) != ERR) {
        switch (pressed_key) {
            set_key('1', 0x1)
            set_key('2', 0x2)
            set_key('3', 0x3)
            set_key('4', 0xC)
            set_key('q', 0x4)
            set_key('w', 0x5)
            set_key('e', 0x6)
            set_key('r', 0xD)
            set_key('a', 0x7)
            set_key('s', 0x8)
            set_key('d', 0x9)
            set_key('f', 0xE)
            set_key('z', 0xA)
            set_key('x', 0x0)
            set_key('c', 0xB)
            set_key('v', 0xF)
            default:
                break;
        }
    }
#endif


    return 0;
}



int draw_screen() {
#ifdef NCURSES
    wmove(chip_window, 0, 0);
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
                waddch(chip_window, ' ' | COLOR_PAIR(screen[IND(y, x)]));
                waddch(chip_window, ' ' | COLOR_PAIR(screen[IND(y, x)]));
        }
        wmove(chip_window, y+1, 0);
    }
    
#endif
    return 0;
}

int run_chip() {

    struct timeval prev;
    gettimeofday(&prev, NULL);
    uint64_t time_elapsed_frame = 0;
    uint64_t time_elapsed_timer = 0;

    for (;;) {

#ifdef NCURSES
        fat8_export_framebuffer(screen);
        draw_screen();
        wrefresh(stdscr);
        wrefresh(chip_window);
#endif
        struct timeval curr;
        gettimeofday(&curr, NULL);

        uint64_t time_delta = curr.tv_sec - prev.tv_sec;
        time_delta *= 1000000;
        time_delta += curr.tv_usec - prev.tv_usec;

        time_elapsed_frame += time_delta;
        time_elapsed_timer += time_delta;

        get_keys();

        if (time_elapsed_frame > frame_time) {
            time_elapsed_frame -= frame_time;
            fat8_operation();

            clear_keys();
        }
        if (time_elapsed_timer > timer_time) {
            time_elapsed_timer -= timer_time;
            fat8_timers();
        }

        prev = curr;
    }

    return 0;
}


