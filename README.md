# FAT-8

Simple CHIP-8 interpreter written in C using ncurses for graphics and keyboard handling.

## Installation and running

To compile the program run ``make``

Usage: ``bin/fat-8 ROM_PATH [SPEED_MULTIPLIER]``

CHIP-8 delay and sound timers run at 60 Hz, default execution speed of FAT-8 is also 60 Hz, to make it run faster pass an
optional command line argument SPEED_MULTIPLIER.

## Sources

ROM collections:

[](https://github.com/kripod/chip8-roms)

CHIP-8 specification based on:

[Timendus' Chip Test Suite](https://github.com/Timendus/chip8-test-suite) and various sources referenced there.

[CHIP‐8 Technical Reference (Matthew Mikolay)](https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Technical-Reference)

Following resources were used at the beggining of this project but were found to by largely innacurate and were therefore abandoned:

[Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.4)

[Building a CHIP-8 Emulator [C++] (Austin Morlan)](https://austinmorlan.com/posts/chip8_emulator/)

## TODO

- add SDL/OpenGL mode with sound
- smooth out handling of keypad by ncurses (if possible ((not really)))
