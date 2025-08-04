#include "chip_8.h"
#include "instructions.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

const unsigned int chip_8_font[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,   // 0
    0x20, 0x60, 0x20, 0x20, 0x70,   // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,   // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,   // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,   // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,   // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,   // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,   // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,   // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,   // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,   // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,   // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,   // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,   // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,   // E
    0xF0, 0x80, 0xF0, 0x80, 0x80    // F
};

void decode(Chip_8 *chip_8, uint16_t inst)
{
    uint8_t op = (inst >> 12) & 0xF;
    int last;
    switch (op) {
        case 0x0:
            if ((inst & 0x0FFF) == 0x0E0) {
                CLS(chip_8);
            } else if ((inst & 0x0FFF) == 0x0EE) {
                RET(chip_8);
            }
            break;
        case 0x1:
            JP_ADDR(chip_8, inst);
            break;
        case 0x2:
            CALL(chip_8, inst);
            break;
        case 0x3:
            SE(chip_8, inst);
            break;
        case 0x4:
            SNE(chip_8, inst);
            break;
        case 0x5:
            SE_VX_VY(chip_8, inst);
            break;
        case 0x6:
            LD_VX(chip_8, inst);
            break;
        case 0x7:
            ADD_VX(chip_8, inst);
            break;
        case 0x8:
            last = (inst & 0xF);
            switch (last) {
                case 0x0:
                    LD(chip_8, inst);
                    break;
                case 0x1:
                    OR(chip_8, inst);
                    break;
                case 0x2:
                    AND(chip_8, inst);
                    break;
                case 0x3:
                    XOR(chip_8, inst);
                    break;
                case 0x4:
                    ADD(chip_8, inst);
                    break;
                case 0x5:
                    SUB(chip_8, inst);
                    break;
                case 0x6:
                    SHR(chip_8, inst);
                case 0x7:
                    SUBN(chip_8, inst);
                    break;
                case 0xE:
                    SHL(chip_8, inst);
                    break;
                default:
                    printf("[INFO]: Instruction not implemented\n");
            }
        case 0xA:
            LD_I(chip_8, inst);
            break;
        case 0xD:
            DRW(chip_8, inst);
            break;
    }
}

uint16_t fetch(Chip_8 *chip_8)
{
    uint16_t inst = (chip_8->memory[chip_8->PC] << 8) | (chip_8->memory[chip_8->PC+1] & 0xFF);
    chip_8->PC += 2;
    return inst;
}

void print_display(Chip_8 chip_8)
{
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            printf("%d ", chip_8.display[x * SCREEN_WIDTH + y]);
        }
        printf("\n");
    }
}

void print_registers(Chip_8 chip_8)
{
    printf("**************************\n");
    printf("[PC] 0x%X\n", chip_8.PC);
    for (int i = 0; i < N_REGISTERS; i++) {
        printf("[V%X] 0x%X\n", i, chip_8.V[i]);
    }
    printf("[I] 0x%X\n", chip_8.I);
    printf("**************************\n");
}

void print_memory(Chip_8 chip_8)
{
    for (int i = 0x50; i < 0x283; i++) {
        printf("[0x%X] 0x%02X\n", i, chip_8.memory[i]);
    }
}

void load_font(Chip_8 *chip_8)
{
    int j = 0;
    for (int i = 0x50; i <= 0x9F; i++)
        chip_8->memory[i] = chip_8_font[j++];
}

void load_rom(Chip_8 *chip_8, const char *rom)
{
    load_font(chip_8);
    FILE *fp = fopen(rom, "rb");

    if (fp == NULL) {
        fprintf(stderr, "Error: could not open %s\n", rom);
        exit(1);
    }

    fprintf(stdout, "[INFO]: %s opened correctly\n", rom);

    uint8_t byte;
    int i = 0;

    while (fread(&byte, sizeof(uint8_t), 1, fp) == 1) {
        chip_8->memory[START_PC + i++] = byte;
    }

    fclose(fp);
}



Chip_8 init_chip_8(void)
{
    Chip_8 chip_8 = {
        .memory = {0},
        .V = {0},
        .stack = {0},
        .I = 0,
        .SP = 0,
        .PC = START_PC,
        .s_timer = 0,
        .d_timer = 0
    };
    return chip_8;
}
