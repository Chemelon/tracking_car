#ifndef __FUNCLIST_H__
#define __FUNCLIST_H__
#include "stm32f10x.h"
#include "tracking.h"

#define FUNC_0X00 tracking_straight
#define FUNC_0X01 NULL
#define FUNC_0X02 NULL
#define FUNC_0X03 NULL
#define FUNC_0X04 NULL
#define FUNC_0X05 NULL
#define FUNC_0X06 NULL
#define FUNC_0X07 NULL
#define FUNC_0X08 NULL
#define FUNC_0X09 NULL


void FunList_Call(uint8_t code);

#endif
