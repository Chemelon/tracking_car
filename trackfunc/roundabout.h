#ifndef __ROUNDABOUT_H
#define __ROUNDABOUT_H
#include "usart.h"

#define ROUND_LEFTBASE 18000
#define ROUND_RIGHTBASE 18000

#define ROUND_LEFTANGLE 75
#define ROUND_LEFTADD 2000
#define ROUND_LEFTSUB 2000

#define ROUND_RIGHTANGLE 90
#define ROUND_RIGHTADD 2000
#define ROUND_RIGHTSUB 2000

/* log开关 */
#define DEBUG_ROUNDABOUT 1

#if DEBUG_ROUNDABOUT
#define ROUNDABOUT_LOG(msg) Usart_SendString(DEBUG_USARTx, msg)
#else
#define ROUNDABOUT_LOG(msg)
#endif

typedef enum roundabout_posit
{
    posit_left,
    posit_right
} roundabout_posit_type;


void beforeround(void);
void circle_in(roundabout_posit_type round_posit);

#endif

