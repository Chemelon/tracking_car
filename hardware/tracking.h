#ifndef __TRACKING_H
#define __TRACKING_H
#include "stm32f10x.h"

/**
 * @brief 选择获取光电数据的方式 1 轮询 0 外部中断
 * 
 */
#define TRACKER_POLLING 1

/* 比例 */
#define KP 1
/* 积分 */
#define KI 2
/* 微分 */
#define KD 3

/**
 * @brief 光电状态枚举变量
 * 
 */
typedef enum tracker_update_status
{
    tracker_updated,
    tracker_resloved
}tracker_update_status_type;

/**
 * @brief 储存光电管状态
 * 
 */
typedef struct tracker_status_struct{
    volatile tracker_update_status_type update;
    volatile uint32_t tarcker1_status;
    volatile uint32_t tarcker2_status;
    volatile uint32_t tarcker3_status;
    volatile uint32_t tarcker4_status;
    volatile uint32_t tarcker5_status;
    volatile uint32_t tracker_cnt_it;
    volatile int32_t tracker_sum_signed;
}tracker_type;

/**
 * @brief 指向光电管状态的指针
 * 
 */
extern volatile tracker_type * ptracker_status;

void GPIO_tracker_init(void);
void NVIC_tracker_init(void);
void GPIO_tracker_init_polling(void);
void TIM3_tracker_init_polling(void);
void NVIC_tracker_init_polling(void);
void USART_sendinfo(void);
int32_t caclu_pid(void);

#endif
