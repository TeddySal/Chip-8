#include "raylib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define PIXEL_SIZE 10  // Scale for visibility
#define MEMORY_SIZE 4096
#define NUM_REGISTERS 16
#define STACK_SIZE 16
#define NUM_KEYS 16
#define PROGRAM_START 0x200

// CHIP-8 State
uint8_t memory[MEMORY_SIZE] = {0};   // 4KB Memory
uint8_t V[NUM_REGISTERS] = {0};      // 16 Registers
uint16_t I = 0;                      // Index Register
uint16_t pc = PROGRAM_START;         // Program Counter
uint8_t delay_timer = 0, sound_timer = 0;
uint16_t stack[STACK_SIZE] = {0};    // Stack
uint8_t sp = 0;                      // Stack Pointer
uint8_t keypad[NUM_KEYS] = {0};      // Keypad
uint8_t gfx[SCREEN_WIDTH * SCREEN_HEIGHT] = {0};  // Display (1D buffer)
uint16_t opcode;                     // Current Opcode

// CHIP-8 Fontset (0-F Digits)
uint8_t fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// CHIP-8 Display
void draw_chip8_display() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            if (gfx[y * SCREEN_WIDTH + x]) {
                DrawRectangle(x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, WHITE);
            }
        }
    }
}

// Load a CHIP-8 ROM
int load_rom(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open ROM: %s\n", filename);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    size_t rom_size = ftell(file);
    rewind(file);

    if (rom_size > (MEMORY_SIZE - PROGRAM_START)) {
        printf("ROM too large\n");
        fclose(file);
        return 0;
    }

    fread(&memory[PROGRAM_START], rom_size, 1, file);
    fclose(file);
    return 1;
}

// Initialize CHIP-8
void chip8_init() {
    pc = PROGRAM_START;
    I = 0;
    sp = 0;
    delay_timer = sound_timer = 0;
    for (int i = 0; i < 80; i++) memory[i] = fontset[i]; // Load fontset into memory
}

// Emulate one cycle of CHIP-8 CPU
void chip8_emulate_cycle() {
    opcode = (memory[pc] << 8) | memory[pc + 1]; // Fetch
    pc += 2;

    switch (opcode & 0xF000) {
        case 0x00E0: // 00E0: Clear screen
            for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) gfx[i] = 0;
            break;

        case 0xD000: { // DXYN: Draw sprite at (VX, VY) with N height
            uint8_t VX = (opcode >> 8) & 0xF;
            uint8_t VY = (opcode >> 4) & 0xF;
            uint8_t N = opcode & 0xF;
            uint8_t x = V[VX] & 63, y = V[VY] & 31;
            V[0xF] = 0;

            for (uint8_t row = 0; row < N; row++) {
                if (y + row >= SCREEN_HEIGHT) break;
                uint8_t sprite_byte = memory[I + row];

                for (uint8_t col = 0; col < 8; col++) {
                    if (x + col >= SCREEN_WIDTH) break;
                    uint8_t sprite_pixel = (sprite_byte >> (7 - col)) & 1;
                    uint16_t screen_index = (y + row) * SCREEN_WIDTH + (x + col);

                    if (sprite_pixel) {
                        if (gfx[screen_index]) {
                            gfx[screen_index] = 0;
                            V[0xF] = 1;
                        } else {
                            gfx[screen_index] = 1;
                        }
                    }
                }
            }
            break;
        }

        default:
            printf("Unknown opcode: 0x%X\n", opcode);
            break;
    }

    if (delay_timer > 0) delay_timer--;
    if (sound_timer > 0) sound_timer--;
}

// Main
int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <ROM>\n", argv[0]);
        return 1;
    }

    InitWindow(SCREEN_WIDTH * PIXEL_SIZE, SCREEN_HEIGHT * PIXEL_SIZE, "CHIP-8 Emulator (Raylib)");
    SetTargetFPS(60);
    chip8_init();

    if (!load_rom(argv[1])) {
        CloseWindow();
        return 1;
    }

    while (!WindowShouldClose()) {
        chip8_emulate_cycle();

        BeginDrawing();
        ClearBackground(BLACK);
        draw_chip8_display();
        EndDrawing();

        usleep(100000);
    }

    CloseWindow();
    return 0;
}
