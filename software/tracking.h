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
    volatile int32_t tracker_sum;
} tracker_type;

enum tracker_color
{
    t_color_white,
    t_color_black
};

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
void func_caller(void);


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
#define POLLING_CNT 0

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


#define DEBUG_ACTIONSTOP    \
    do                      \
    {                       \
        brake();             \
        tracker_sendinfo(); \
        while (1)           \
            ;               \
    } while (0)
#endif
