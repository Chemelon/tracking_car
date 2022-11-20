#ifndef __TRACKING_H
#define __TRACKING_H
#include "stm32f10x.h"

typedef enum tracker_update_status
{
    tracker_updated,
    tracker_resloved
}tracker_update_status_type;


void GPIO_tracker_init(void);
void NVIC_tracker_init(void);

#endif
