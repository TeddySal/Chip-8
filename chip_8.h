#ifndef CHIP_8_CORE
#define CHIP_8_CORE
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define PIXEL_SIZE 20
#define MEMORY_CAP 4096
#define N_REGISTERS 16
#define STACK_CAP 16
#define KEYBOARD_CAP 16
#define DISPLAY_CAP 64*32
#define START_PC 0x200

//unsigned short sprite_font = 70;

struct __chip_8__{
    unsigned char memory[MEMORY_CAP];
    unsigned char V[N_REGISTERS];
    unsigned short stack[STACK_CAP];
    unsigned short I;
    unsigned short PC;      // program counter
    unsigned char SP;       // stack pointer
    unsigned char s_timer;  // sound timer
    unsigned char d_timer;  // delay timer
    unsigned int keyboard[KEYBOARD_CAP];
    bool display[DISPLAY_CAP];
};

typedef struct __chip_8__ Chip_8;

// Chip-8 functions
unsigned short fetch(Chip_8*);
void decode(Chip_8*, unsigned short);
void execute(Chip_8*);
void load_rom(Chip_8*, const char *rom);
void load_font(Chip_8*);
void print_memory(Chip_8);
void print_registers(Chip_8);
void print_display(Chip_8);
Chip_8 init_chip_8(void);

#endif
