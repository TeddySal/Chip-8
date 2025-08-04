#include "instructions.h"

void DRW(Chip_8 *chip_8, uint16_t inst)
{
    uint8_t X = chip_8->V[(inst >> 8) & 0xF] % 64;
    uint8_t Y = chip_8->V[(inst >> 4) & 0xF] % 32;
    uint8_t N = inst & 0xF;
    uint8_t pixel;

    chip_8->V[0xF] = 0;
    for (int yline = 0; yline < N; yline++) {
        pixel = chip_8->memory[chip_8->I + yline];

        for(int xline = 0; xline < 8; xline++) {
            if((pixel & (0x80 >> xline)) != 0) {
                if(chip_8->display[(X + xline + ((Y + yline) * 64))] == 1) {
                    chip_8->V[0xF] = 1;
                }
                chip_8->display[X + xline + ((Y + yline) * 64)] ^= 1;
            }
        }
    }
}

void LD_I(Chip_8* chip_8, unsigned short inst)
{
    chip_8->I = inst & 0x0FFF;
}

void ADD_VX(Chip_8* chip_8, unsigned short inst)
{
    chip_8->V[(inst >> 8) & 0xF] +=  inst & 0xFF;
}

void LD_VX(Chip_8* chip_8, unsigned short inst)
{
    chip_8->V[(inst >> 8) & 0xF] = inst & 0xFF;
}

void SHL(Chip_8 *chip_8, unsigned short inst)
{
    chip_8->V[(inst >> 4) & 0xF] = (chip_8->V[(inst >> 8) & 0xF] >> 15) & 1;
    chip_8->V[(inst >> 8) & 0xF] *= 2;
}

void SUBN(Chip_8 *chip_8, unsigned short inst)
{
    if (chip_8->V[(inst >> 4) & 0xF] >  chip_8->V[(inst >> 8) & 0xF])
        chip_8->V[0xF] = 1;
    else
        chip_8->V[0xF] = 0;
    chip_8->V[(inst >> 8) & 0xF] = chip_8->V[(inst >> 4) & 0xF] - chip_8->V[(inst >> 8) & 0xF];
}

void SHR(Chip_8 *chip_8, unsigned short inst)
{
    chip_8->V[(inst >> 4) & 0xF] = chip_8->V[(inst >> 8) & 0xF] & 1;
    chip_8->V[(inst >> 8) & 0xF] /= 2;
}

void SUB(Chip_8 *chip_8, unsigned short inst)
{
    if (chip_8->V[(inst >> 8) & 0xF] > chip_8->V[(inst >> 4) & 0xF])
        chip_8->V[0xF] = 1;
    else
        chip_8->V[0xF] = 0;
    chip_8->V[(inst >> 8) & 0xF] -= chip_8->V[(inst >> 4) & 0xF];
}

void ADD(Chip_8 *chip_8, unsigned short inst)
{
    chip_8->V[(inst >> 8) & 0xF] += chip_8->V[(inst >> 4) & 0xF];
    /*if (ris > 255)  chip_8->V[0xF] = 1;
    else            chip_8->V[0xF] = 0;*/
}

void XOR(Chip_8 *chip_8, unsigned short inst)
{
    chip_8->V[(inst >> 8) & 0xF] ^= chip_8->V[(inst >> 4) & 0xF];
}

void AND(Chip_8 *chip_8, unsigned short inst)
{
    chip_8->V[(inst >> 8) & 0xF] &= chip_8->V[(inst >> 4) & 0xF];
}

void OR(Chip_8 *chip_8, unsigned short inst)
{
    chip_8->V[(inst >> 8) & 0xF] |= chip_8->V[(inst >> 4) & 0xF];
}

void LD(Chip_8 *chip_8, unsigned short inst)
{
    chip_8->V[(inst >> 8) & 0xF] = chip_8->V[(inst >> 4) & 0xF];
}

void SE_VX_VY(Chip_8 *chip_8, unsigned short inst)
{
    if (chip_8->V[(inst >> 8) & 0xF] == chip_8->V[(inst & 0xFF)]) {
        chip_8->PC += 2;
    }
}

void SNE(Chip_8 *chip_8, unsigned short inst)
{
    if (chip_8->V[(inst >> 8) & 0xF] != (inst & 0xFF)) {
        chip_8->PC += 2;
    }
}

void SE(Chip_8 *chip_8, unsigned short inst)
{
    if (chip_8->V[(inst >> 8) & 0xF] == (inst & 0xFF)) {
        chip_8->PC += 2;
    }
}

void CALL(Chip_8 *chip_8, unsigned short inst)
{
    chip_8->SP++;
    chip_8->stack[chip_8->SP] = chip_8->PC;
    chip_8->PC = inst & 0x0FFF;
}

void JP_ADDR(Chip_8 *chip_8, uint16_t inst)
{
    chip_8->PC = inst & 0x0FFF;
}

void RET(Chip_8 *chip_8)
{
    chip_8->PC = chip_8->stack[chip_8->SP--];
}

void CLS(Chip_8 *chip_8)
{
    for (int i = 0; i < DISPLAY_CAP; i++)
        chip_8->display[i] = 0;
}
