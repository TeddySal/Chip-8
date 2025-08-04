#ifndef CHIP_8_INSTRUCTIONS
#define CHIP_8_INSTRUCTIONS
#include "chip_8.h"
#include <assert.h>

void CLS(Chip_8*);
void RET(Chip_8*);
void CALL(Chip_8*, unsigned short);
void SE(Chip_8*, unsigned short);
void SNE(Chip_8*, unsigned short);
void SE_VX_VY(Chip_8*, unsigned short);
void LD(Chip_8*, unsigned short);
void OR(Chip_8*, unsigned short);
void AND(Chip_8*, unsigned short);
void XOR(Chip_8*, unsigned short);
void ADD(Chip_8*, unsigned short);
void SUB(Chip_8*, unsigned short);
void SHR(Chip_8*, unsigned short);
void SUBN(Chip_8*, unsigned short);
void SHL(Chip_8*, unsigned short);
void JP_ADDR(Chip_8*, unsigned short);
void LD_VX(Chip_8*, unsigned short);
void ADD_VX(Chip_8*, unsigned short);
void LD_I(Chip_8*, unsigned short);
void DRW(Chip_8*, unsigned short);

#endif
