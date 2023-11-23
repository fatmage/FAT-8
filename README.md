# FAT-8

Simple CHIP-8 interpreter written in C using ncurses.

## Installation and running

To compile the program run ``make``

Usage: ``bin/fat-8 ROM_PATH [SPEED_MULTIPLIER]``

## Sources

CHIP-8 specification based on:

[CHIP‐8 Technical Reference (Matthew Mikolay)](https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Technical-Reference)

[Timendus' Chip Test Suite](https://github.com/Timendus/chip8-test-suite)

Following resources were used at the beggining of this project but were found to by largely innacurate and were therefore abandoned:

[Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.4)

[Building a CHIP-8 Emulator [C++] (Austin Morlan)](https://austinmorlan.com/posts/chip8_emulator/)

## TODO

- fix bugs (space invaders)
- add SDL/OpenGL mode with sound
- smooth out handling of keypad by ncurses (if possible)
