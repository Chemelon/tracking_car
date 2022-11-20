#ifndef __FUNCLIST_H__
#define __FUNCLIST_H__
#include "stm32f10x.h"

void Fun_HandShake_00h(uint8_t FunCode);
void Fun_DisplayConfig_10h(uint8_t FunCode);
void Fun_ShowFont_20h(uint8_t FunCode);
void Fun_LCDClear_65h(uint8_t FunCode);
void Fun_FillRectangle_66h(uint8_t FunCode);
void Fun_DisplayPic_91h(uint8_t FunCode);
void Fun_SetDisplayDir_92h(uint8_t FunCode);
void Fun_WR_EE_94h(uint8_t FunCode);
void Fun_RD_EE_95h(uint8_t FunCode);
void Fun_WR_FLASH_96h(uint8_t FunCode);
void Fun_RD_FLASH_97h(uint8_t FunCode);
void Fun_FileDownLoad_98h(uint8_t FunCode);

#endif
