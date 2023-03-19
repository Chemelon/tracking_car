#include "tracking.h"
#include "straight.h"
#include "motor.h"
#include "pid.h"
#include "math.h"

/* 直线循迹 */
void tracking_straight(void)
{
    servo_setangle(S_STRAIGHTWARD);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        /* 出口条件 */
        if ((TRACKER1_STATUS == t_color_black) || (TRACKER5_STATUS == t_color_black))
        {
            // tracker_sendinfo();
            if (TRACKER3_STATUS == t_color_white)
            {
                brake();
                tracking_resume();
                servo_setangle(S_STRAIGHTWARD);
                STRAIGHT_LOG("STRAITHTEXIT\r\n");
#if DEBUG_STRAIGHT
                DEBUG_ACTIONSTOP;
#endif
                break;
            }
        }
        if (TRACKER4_STATUS == t_color_black)
        {
            /* 偏左 向右修正*/
            servo_setangle(S_RIGHTWARD);
            motor_setforward_left(STRAIGHTBASE_LEFT + RIGHTWARD_ADD);
            motor_setforward_right(STRAIGHTBASE_RIGHT);
            STRAIGHT_LOG("RIGHTWARD\r\n");
        }
        else if (TRACKER2_STATUS == t_color_black)
        {
            /* 偏右 向左修正*/
            servo_setangle(S_LEFTWARD);
            motor_setforward_left(STRAIGHTBASE_LEFT);
            motor_setforward_right(STRAIGHTBASE_RIGHT + LEFTWARD_ADD);
            STRAIGHT_LOG("LEFTWARD\r\n");
        }
        // STRAIGHT_LOG("STRAIGHTING\r\n");
        tracking_resume();
    }
}

/* 直线循迹 */
void tracking_cross(void)
{
    servo_setangle(S_STRAIGHTWARD);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        if (TRACKER1_STATUS + TRACKER2_STATUS + TRACKER3_STATUS + TRACKER4_STATUS + TRACKER5_STATUS > 2)
        {
            if (TRACKER3_STATUS == t_color_black)
            // if (TRACKER2_STATUS == t_color_black && TRACKER3_STATUS == t_color_black && TRACKER4_STATUS == t_color_black)
            {
                brake();
                tracking_resume();
                servo_setangle(S_STRAIGHTWARD);
                STRAIGHT_LOG("CROSSEXIT\r\n");
#if DEBUG_STRAIGHT
                // DEBUG_ACTIONSTOP;
#endif
                break;
            }
        }
        // STRAIGHT_LOG("UPDATED\r\n");
        if (TRACKER4_STATUS == t_color_black)
        {
            /* 偏左 向右修正*/
            servo_setangle(95);
            motor_setforward_left(STRAIGHTBASE_LEFT + 2000);
            motor_setforward_right(STRAIGHTBASE_RIGHT - 1000);
            // STRAIGHT_LOG("RIGHTWARD\r\n");
        }
        else if (TRACKER2_STATUS == t_color_black)
        {
            /* 偏右 向左修正*/
            servo_setangle(85);
            motor_setforward_left(STRAIGHTBASE_LEFT - 1000);
            motor_setforward_right(STRAIGHTBASE_RIGHT + 2000);
            // STRAIGHT_LOG("LEFTWARD\r\n");
        }
        // STRAIGHT_LOG("STRAIGHTING\r\n");
        tracking_resume();
    }
}

/* 直线循迹pid */
#define DELTA_MAX 12000
// 往右偏为正
void tracking_straight_pid(void)
{
    pid_type_int straight_pid = {600, 5, 0};
    int32_t delta = 0, angle = 0;
    servo_setangle(S_STRAIGHTWARD);
    STRAIGHT_LOG("STRAITHTIN\r\n");
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }

        if ((TRACKER1_STATUS == t_color_black) || (TRACKER5_STATUS == t_color_black))
        {
            if (TRACKER3_STATUS == t_color_white && (TRACKER2_STATUS == t_color_white || TRACKER4_STATUS == t_color_white))
            {
                brake();

                tracking_resume();
                STRAIGHT_LOG("STRAITHTEXIT\r\n");
                // DEBUG_ACTIONSTOP;
                break;
            }
        }
        delta = positional_pid_int(&straight_pid, (int32_t)0, (int32_t)ptracker_status->tracker_sum);
        if (delta > DELTA_MAX)
        {
            delta = DELTA_MAX;
        }
        else if (delta < (-DELTA_MAX))
        {
            delta = (-DELTA_MAX);
        }
        angle = delta / (DELTA_MAX / 8);
        if (delta > 8000)
            angle = (delta - 1700) / (DELTA_MAX / 15);
        if (delta < 8000)
            angle = (delta + 1700) / (DELTA_MAX / 15);
        {
            motor_setforward_left(((int32_t)STRAIGHTBASE_LEFT - delta) & 0xffff);
            motor_setforward_right(((int32_t)STRAIGHTBASE_RIGHT + delta) & 0xffff);
            servo_setangle(((int32_t)90 - angle) & 0xffff);
        }
        // tracker_sendinfo();
        // printf("%d %d\r\n", delta, angle);
        tracking_resume();
    }
}

/* 按距离屏蔽光电的PID巡线 */
void tracking_straight_pid_s(uint16_t s)
{
    /* 启动定时器 */
    TIM3->CR1 &= ~TIM_CR1_CEN;
    TIM3->CNT = 0;
    TIM3->CR1 |= TIM_CR1_CEN;
    pid_type_int straight_pid = {600, 5, 0, 0, 0, 0};
    int32_t delta = 0, angle = 0;
    STRAIGHT_LOG("STRAITHTIN\r\n");
    servo_setangle(S_STRAIGHTWARD);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        if (TIM3->CNT > s)
        {
            STRAIGHT_LOG("S OK\r\n");
            /* 出口条件 */
            if ((TRACKER1_STATUS == t_color_black) || (TRACKER5_STATUS == t_color_black))
            {
                // tracker_sendinfo();
                if (TRACKER3_STATUS == t_color_white)
                // if (TRACKER3_STATUS == t_color_white && (TRACKER2_STATUS == t_color_white || TRACKER4_STATUS == t_color_white))
                {
                    /* 关闭定时器 */
                    TIM3->CR1 &= ~TIM_CR1_CEN;
                    TIM3->CNT = 0;
                    brake();
                    tracking_resume();
                    STRAIGHT_LOG("STRAITHTEXIT\r\n");
                    // DEBUG_ACTIONSTOP;
                    break;
                }
            }
        }
        delta = positional_pid_int(&straight_pid, (int32_t)0, (int32_t)ptracker_status->tracker_sum);
        if (delta > DELTA_MAX)
        {
            delta = DELTA_MAX;
        }
        else if (delta < (-DELTA_MAX))
        {
            delta = (-DELTA_MAX);
        }
        angle = delta / (DELTA_MAX / 8);
        if (delta > 8000)
            angle = (delta - 1700) / (DELTA_MAX / 15);
        if (delta < 8000)
            angle = (delta + 1700) / (DELTA_MAX / 15);
        {
            motor_setforward_left(((int32_t)STRAIGHTBASE_LEFT - delta) & 0xffff);
            motor_setforward_right(((int32_t)STRAIGHTBASE_RIGHT + delta) & 0xffff);
            servo_setangle(((int32_t)90 - angle) & 0xffff);
        }
        // tracker_sendinfo();
        // printf("%d %d\r\n", delta, angle);
        tracking_resume();
    }
}

pid_type_int independent_pid = {22, 5, 10, 8500, -8500, 0};
#define NEW_MAX 12000
/* 按距离屏蔽光电 且巡线固定距离的高速PID巡线 */
void tracking_straightfast_pid_sm(uint16_t s, uint16_t m, int32_t basespeed)
{
    // int32_t basespeed = 20000;
    /* 启动定时器 */
    TIM3->CR1 &= ~TIM_CR1_CEN;
    TIM3->CNT = 0;
    TIM3->CR1 |= TIM_CR1_CEN;

    int32_t delta = 0, angle = 0;
    STRAIGHT_LOG("STRAITHTIN\r\n");
    servo_setangle(S_STRAIGHTWARD);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        /* 最大路程停车 */
        if (TIM3->CNT > m)
        {
            /* 关闭定时器 */
            TIM3->CR1 &= ~TIM_CR1_CEN;
            TIM3->CNT = 0;
            brake();
            tracking_resume();
            STRAIGHT_LOG("STRAITHTEXIT\r\n");
            //DEBUG_ACTIONSTOP;
            break;
        }
        /* 屏蔽结束 */
        if (TIM3->CNT > s)
        {
            /* 出口条件 */
            if ((TRACKER1_STATUS == t_color_black) || (TRACKER5_STATUS == t_color_black))
            {
                if (TRACKER3_STATUS == t_color_white)
                {
                    /* 关闭定时器 */
                    TIM3->CR1 &= ~TIM_CR1_CEN;
                    TIM3->CNT = 0;
                    brake();
                    tracking_resume();
                    STRAIGHT_LOG("STRAITHTEXIT\r\n");
                    break;
                }
            }
        }
        delta = positional_pid_int_independent(&independent_pid, (int32_t)0, ptracker_status->tracker_sum);
        if (delta > NEW_MAX)
        {
            delta = NEW_MAX;
        }
        else if (delta < (-NEW_MAX))
        {
            delta = (-NEW_MAX);
        }
        angle = delta / (NEW_MAX / 25);
        // if (delta > 8000)
        //     angle = (delta - 2000) / (NEW_MAX / 15);
        // if (delta < 8000)
        //     angle = (delta + 2000) / (NEW_MAX / 15);

        motor_setforward_left((basespeed - delta) & 0xffff);
        motor_setforward_right((basespeed + delta) & 0xffff);
        servo_setangle(((int32_t)90 - angle) & 0xffff);
        tracking_resume();
    }
}

void tracking_cross_pid(void)
{
    /* 启动定时器 */
    TIM3->CR1 &= ~TIM_CR1_CEN;
    TIM3->CNT = 0;
    TIM3->CR1 |= TIM_CR1_CEN;
    pid_type_int straight_pid = {750, 5, 0};
    int32_t delta = 0, angle = 0;
    servo_setangle(S_STRAIGHTWARD);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        if (TRACKER1_STATUS + TRACKER2_STATUS + TRACKER3_STATUS + TRACKER4_STATUS + TRACKER5_STATUS > 2)
        // if (TIM3->CNT > 350 && TRACKER3_STATUS == t_color_black)
        //  if (TRACKER1_STATUS == t_color_black || TRACKER5_STATUS == t_color_black)
        {
            if (TRACKER3_STATUS == t_color_black)
            {
                /* 关闭定时器 */
                TIM3->CR1 &= ~TIM_CR1_CEN;
                TIM3->CNT = 0;
                brake();
                tracking_resume();
                // DEBUG_ACTIONSTOP;
                break;
            }
        }
        delta = positional_pid_int(&straight_pid, (int32_t)0, (int32_t)ptracker_status->tracker_sum);
        if (delta > DELTA_MAX)
        {
            delta = DELTA_MAX;
        }
        else if (delta < (-DELTA_MAX))
        {
            delta = (-DELTA_MAX);
        }
        angle = delta / (DELTA_MAX / 10);
        if (delta > 8000)
            angle = (delta - 2000) / (DELTA_MAX / 10);
        if (delta < 8000)
            angle = (delta + 2000) / (DELTA_MAX / 10);
        {
            motor_setforward_left(((int32_t)STRAIGHTBASE_LEFT - delta) & 0xffff);
            motor_setforward_right(((int32_t)STRAIGHTBASE_RIGHT + delta) & 0xffff);
            servo_setangle(((int32_t)90 - angle) & 0xffff);
        }
        // tracker_sendinfo();
        // printf("%d %d\r\n", delta, angle);
        tracking_resume();
    }
}

void tracking_final_pid(void)
{
    /* 启动定时器 */
    TIM3->CR1 &= ~TIM_CR1_CEN;
    TIM3->CNT = 0;
    TIM3->CR1 |= TIM_CR1_CEN;
    pid_type_int straight_pid = {750, 5, 0};
    int32_t delta = 0, angle = 0;
    servo_setangle(S_STRAIGHTWARD);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        if (TIM3->CNT > 2800 && TRACKER3_STATUS == t_color_black)
        // if (TRACKER1_STATUS == t_color_black || TRACKER5_STATUS == t_color_black)
        {
            // if (TRACKER3_STATUS == t_color_black)
            {
                /* 关闭定时器 */
                TIM3->CR1 &= ~TIM_CR1_CEN;
                TIM3->CNT = 0;
                brake();
                tracking_resume();
                // DEBUG_ACTIONSTOP;
                break;
            }
        }
        delta = positional_pid_int(&straight_pid, (int32_t)0, (int32_t)ptracker_status->tracker_sum);
        if (delta > DELTA_MAX)
        {
            delta = DELTA_MAX;
        }
        else if (delta < (-DELTA_MAX))
        {
            delta = (-DELTA_MAX);
        }
        angle = delta / (DELTA_MAX / 10);
        if (delta > 8000)
            angle = (delta - 2000) / (DELTA_MAX / 10);
        if (delta < 8000)
            angle = (delta + 2000) / (DELTA_MAX / 10);
        {
            motor_setforward_left(((int32_t)STRAIGHTBASE_LEFT - delta) & 0xffff);
            motor_setforward_right(((int32_t)STRAIGHTBASE_RIGHT + delta) & 0xffff);
            servo_setangle(((int32_t)90 - angle) & 0xffff);
        }
        // tracker_sendinfo();
        // printf("%d %d\r\n", delta, angle);
        tracking_resume();
    }
}
