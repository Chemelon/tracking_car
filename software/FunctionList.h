#ifndef __FUNCLIST_H__
#define __FUNCLIST_H__
#include "stm32f10x.h"
#include "motor.h"
#include "tracking.h"
#include "straight.h"
#include "turning.h"
#include "roundabout.h"
#include "curve-8.h"
#include <stdio.h>

#define FUNC_0X00 tracking_straight
#define FUNC_0X01 tracking_right
#define FUNC_0X02 tracking_left
#define FUNC_0X03 NULL
#define FUNC_0X04 NULL
#define FUNC_0X05 NULL
#define FUNC_0X06 NULL
#define FUNC_0X07 NULL
#define FUNC_0X08 NULL
#define FUNC_0X09 NULL

#define FUNC_0X10 stop
#define FUNC_0X11 NULL
#define FUNC_0X12 NULL
#define FUNC_0X13 NULL
#define FUNC_0X14 NULL
#define FUNC_0X15 NULL
#define FUNC_0X16 NULL
#define FUNC_0X17 NULL
#define FUNC_0X18 NULL
#define FUNC_0X19 NULL


void FunList_Call(uint8_t code);

#endif
