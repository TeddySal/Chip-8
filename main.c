#include <stdio.h>
#include <raylib.h>
#include <unistd.h>
#include "chip_8.h"

void draw_chip8_display(Chip_8 chip_8) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            if (chip_8.display[y * SCREEN_WIDTH + x]) {
                DrawRectangle(x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, RAYWHITE);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Error: not enought parameters\n");
        return 1;
    }

    InitWindow(1280, 720, "Chip-8 emulator");

    SetTargetFPS(60);

    Chip_8 chip_8 = init_chip_8();

    load_rom(&chip_8, argv[1]);

    while (!WindowShouldClose()) {
        uint16_t inst = fetch(&chip_8);
        decode(&chip_8, inst);
        BeginDrawing();
            ClearBackground(BLACK);
            DrawFPS(0, 0);
            draw_chip8_display(chip_8);
        EndDrawing();

        usleep(1200);
    }

    CloseWindow();

    return 0;
}
