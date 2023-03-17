#ifndef __TURNING_H
#define __TURNING_H
#include "usart.h"

/* 寻线90度右转log开关 */
#define DEBUG_TRACKRIGHT90 0
/* 寻线90度左转log开关 */
#define DEBUG_TRACKLEFT90 0


#if DEBUG_TRACKLEFT90
#define TRACKLEFT90_LOG(msg) Usart_SendString(DEBUG_USARTx, msg)
#else
#define TRACKLEFT90_LOG(msg)
#endif
#define S_LEFT90 45
/* 向左转直角弯时差速PWM值 即右比左快的值 CCMR 最大值2000*/
#define LEFTTURN_ADD 2000
/* 向左转直角弯时差速PWM值 即左比右慢的值 */
#define LEFTTURN_SUB 1000
#define LEFTTURNBASE_LEFT STRAIGHTBASE_LEFT
#define RIGHTTURNBASE_RIGHT STRAIGHTBASE_RIGHT



#if DEBUG_TRACKRIGHT90
#define TRACKRIGHT90_LOG(msg) Usart_SendString(DEBUG_USARTx, msg)
#else
#define TRACKRIGHT90_LOG(msg)
#endif
#define S_RIGHT90 135
/* 向右转直角弯时差速PWM值 即左比右快的值 */
#define RIGHTTURN_ADD 2000
/* 向右转直角弯时差速PWM值 即右比左慢的值 */
#define RIGHTTURN_SUB 1000
#define RIGHTTURNBASE_LEFT STRAIGHTBASE_LEFT
#define RIGHTTURNBASE_RIGHT STRAIGHTBASE_RIGHT

void tracking_left(void);
void tracking_right(void);

#endif

