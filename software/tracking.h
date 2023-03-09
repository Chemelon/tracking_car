#ifndef __TRACKING_H
#define __TRACKING_H
#include "stm32f10x.h"

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
void TIM1_tracker_init_polling(void);
void NVIC_tracker_init_polling(void);
void tracking_resume(void);
void tracker_sendinfo(void);
int32_t caclu_pid(void);
void stateswitcher(void);

void tracking_straight(void);
void tracking_left(void);
void tracking_right(void);

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
#define POLLING_FREQ 1000
/* 采样次数 */
#define POLLING_CNT 10

/* 状态列表的状态数 */
#define STATE_NUM 10

#define STATUS_MEMBER          \
    {                          \
        0x00,     /* 直行 */ \
            0x01, /* 左转 */ \
            0x00, /* 直行 */ \
            0x01, /* 左转 */ \
            0x00, /* 直行 */ \
            0x01, /* 左转 */ \
            0x00, /* 直行 */ \
            0x01, /* 左转 */ \
            0x00, /* 直行 */ \
            0x02, /* 右转 */ \
    }

/* 直线循迹log开关 */
#define DEBUG_STRAIGHT 0
/* 寻线90度右转log开关 */
#define DEBUG_TRACKRIGHT90 1
/* 寻线90度左转log开关 */
#define DEBUG_TRACKLEFT90 0

#if DEBUG_STRAIGHT
#define STRAIGHT_LOG(msg) Usart_SendString(DEBUG_USARTx, msg)
#else
#define STRAIGHT_LOG(msg)
#endif
/* 直线时舵机角度 */
#define S_STRAIGHTWARD 0
/* 向右修正时舵机角度 */
#define S_RIGHTWARD 155
/* 向左修正时舵机角度 */
#define S_LEFTWARD 25

/* 向右修正时差速PWM值 即左比右快的值 CCMR 最大值36000*/
#define RIGHTWARD_ADD 8000
/* 向左修正时差速PWM值 即右比左快的值 */
#define LEFTWARD_ADD 8000

#if DEBUG_TRACKLEFT90
#define TRACKLEFT90_LOG(msg) Usart_SendString(DEBUG_USARTx, msg)
#else
#define TRACKLEFT90_LOG(msg)
#endif
/* 向左转直角弯时差速PWM值 即右比左快的值 CCMR 最大值2000*/
#define LEFTTURN_ADD 8000
/* 向左转直角弯时差速PWM值 即左比右慢的值 */
#define LEFTTURN_SUB 10000
#define LEFTTURNBASE_LEFT STRAIGHTBASE_LEFT
#define RIGHTTURNBASE_RIGHT STRAIGHTBASE_RIGHT

#if DEBUG_TRACKRIGHT90
#define TRACKRIGHT90_LOG(msg) Usart_SendString(DEBUG_USARTx, msg)
#else
#define TRACKRIGHT90_LOG(msg)
#endif
/* 向右转直角弯时差速PWM值 即左比右快的值 */
#define RIGHTTURN_ADD 8000
/* 向右转直角弯时差速PWM值 即右比左慢的值 */
#define RIGHTTURN_SUB 10000
#define RIGHTTURNBASE_LEFT STRAIGHTBASE_LEFT
#define RIGHTTURNBASE_RIGHT STRAIGHTBASE_RIGHT

#define DEBUG_ACTIONSTOP    \
    do                      \
    {                       \
        brake();             \
        tracker_sendinfo(); \
        while (1)           \
            ;               \
    } while (0)
#endif
