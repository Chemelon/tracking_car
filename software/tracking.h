#ifndef __TRACKING_H
#define __TRACKING_H
#include "stm32f10x.h"

#define TRACKER1_STATUS (ptracker_status->tarcker1)
#define TRACKER2_STATUS (ptracker_status->tarcker2)
#define TRACKER3_STATUS (ptracker_status->tarcker3)
#define TRACKER4_STATUS (ptracker_status->tarcker4)
#define TRACKER5_STATUS (ptracker_status->tarcker5)

/**
 * @brief 选择获取光电数据的方式 1 轮询 0 外部中断
 *
 */
#define TRACKER_POLLING 1
/* 定时器每秒中断次数,即采样频率 2~100000 */
#define POLLING_FREQ 10000
/* 采样次数 */
#define POLLING_CNT 10

/* 状态列表的状态数 */
#define STATE_NUM 4

#define STATUS_MEMBER          \
    {                          \
        0x00,     /* 直行 */ \
            0x01, /* 左转 */ \
            0x00, /* 直行 */ \
            0x01, /* 左转 */ \
    }

/* 直线循迹log开关 */
#define DEBUG_STRAIGHT 1
/* 寻线90度右转log开关 */
#define DEBUG_TRACKRIGHT90_LOG 1
/* 寻线90度左转log开关 */
#define DEBUG_TRACKLEFT90_LOG 1

#if DEBUG_STRAIGHT
#define STRAIGHT_LOG(msg) Usart_SendString(DEBUG_USARTx, msg)
#else
#define STRAIGHT_LOG(msg)
#endif
/* 直线时舵机角度 */
#define S_STRAIGHTWARD 0
/* 向右修正时舵机角度 */
#define S_RIGHTWARD 0
/* 向左修正时舵机角度 */
#define S_LEFTWARD 0
/* 向右修正时差速PWM值 即左比右快的值 */
#define RIGHTWARD_ADD 2500
/* 向左修正时差速PWM值 即右比左快的值 */
#define LEFTWARD_ADD 1500

#if DEBUG_TRACKLEFT90_LOG
#define TRACKLEFT90_LOG(msg) Usart_SendString(DEBUG_USARTx, msg)
#else
#define TRACKLEFT90_LOG(msg)
#endif
/* 向左转直角弯时差速PWM值 即右比左快的值 */
#define LEFTTURN_ADD 1500
/* 向左转直角弯时差速PWM值 即左比右慢的值 */
#define LEFTTURN_SUB 1500

#if DEBUG_TRACKRIGHT90_LOG
#define TRACKRIGHT90_LOG(msg) Usart_SendString(DEBUG_USARTx, msg)
#else
#define TRACKRIGHT90_LOG(msg)
#endif
/* 向左转直角弯时差速PWM值 即右比左快的值 */
#define LEFTTURN_ADD 1500
/* 向左转直角弯时差速PWM值 即左比右慢的值 */
#define LEFTTURN_SUB (-1500)

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
    status_updated,
    status_resloved
} tracker_update_status_type;
/**
 * @brief 储存光电管状态
 *
 */
typedef struct tracker_status_struct
{
    volatile tracker_update_status_type update;
    volatile uint32_t tarcker1;
    volatile uint32_t tarcker2;
    volatile uint32_t tarcker3;
    volatile uint32_t tarcker4;
    volatile uint32_t tarcker5;
    volatile uint32_t tracker_cnt_it;
    volatile int32_t tracker_sum_signed;
} tracker_type;

/**
 * @brief 指向光电管状态的指针
 *
 */
extern volatile tracker_type *ptracker_status;
void GPIO_tracker_init(void);
void NVIC_tracker_init(void);
void GPIO_tracker_init_polling(void);
void TIM3_tracker_init_polling(void);
void NVIC_tracker_init_polling(void);
void tracker_sendinfo(void);
int32_t caclu_pid(void);
void stateswitcher(void);

void tracking_straight(void);
void tracking_left(void);
void tracking_right(void);

#endif
