#include "tracking.h"
#include "roundabout.h"
#include "motor.h"

/* 环前(就是直线循迹不过出口条件不同) */
void beforeround(void)
{
    servo_setangle(90);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        if ((TRACKER1_STATUS == t_color_black) && (TRACKER3_STATUS == t_color_black))
        {
            /* 根据实际数据修改的条件 OK */
            if (TRACKER3_STATUS == t_color_black)
            {
                /* 入弯前走一段直线 */
                gostraight(0);
                servo_setangle(88);
                /* 启动定时器 */
                TIM3->CR1 &= ~TIM_CR1_CEN;
                TIM3->CNT = 0;
                TIM3->CR1 |= TIM_CR1_CEN;
                for (; TIM3->CNT < 370;)
                {
                    tracking_resume();
                }
                /* 关闭定时器 */
                TIM3->CR1 &= ~TIM_CR1_CEN;
                TIM3->CNT = 0;
                brake();
                // DEBUG_ACTIONSTOP;
                break;
            }
        }
        if (TRACKER4_STATUS == t_color_black)
        {
            /* 偏左 向右修正*/
            servo_setangle(95);
            motor_setforward_left(STRAIGHTBASE_LEFT + 2000);
            motor_setforward_right(STRAIGHTBASE_RIGHT);
        }
        else if (TRACKER2_STATUS == t_color_black)
        {
            /* 偏右 向左修正*/
            servo_setangle(85);
            motor_setforward_left(STRAIGHTBASE_LEFT);
            motor_setforward_right(STRAIGHTBASE_RIGHT + 2000);
        }
        tracking_resume();
    }
}

/* 进环 */
void circle_in(roundabout_posit_type round_posit)
{
    //static uint8_t cnt = 0;
    /* 启动定时器 */
    TIM3->CR1 &= ~TIM_CR1_CEN;
    TIM3->CNT = 0;
    TIM3->CR1 |= TIM_CR1_CEN;
    switch (round_posit)
    {
    case posit_left:
    {
        for (;;)
        {
            if (ptracker_status->update == status_resloved)
            {
                continue;
            }
            /* 出口条件 */
            if ((TIM3->CNT > 9350) && TRACKER3_STATUS == t_color_black)
            {
                tracking_resume();
                /* 关闭定时器 */
                TIM3->CR1 &= ~TIM_CR1_CEN;
                TIM3->CNT = 0;
                /* 出弯时 */
                brake();
                //DEBUG_ACTIONSTOP;
                break;
            }
            if (TIM3->CNT > 8100)
            {
                //DEBUG_ACTIONSTOP;
                if (TRACKER4_STATUS == t_color_black || TRACKER3_STATUS == t_color_black)
                {
                    /* 沿外圈循迹 出环 */
                    servo_setangle(93);
                    motor_setforward_left(ROUND_LEFTBASE - 2500);
                    motor_setforward_right(ROUND_RIGHTBASE - 4000);
                }
                else //if (TRACKER2_STATUS == t_color_white)
                {
                    /* 向内修正 */
                    servo_setangle(77);
                    motor_setforward_right(ROUND_LEFTBASE - 1000);
                    motor_setforward_left(ROUND_LEFTBASE - 4000);
                }
            }
            else
            {
                if (TRACKER2_STATUS == t_color_black || TRACKER3_STATUS == t_color_black)
                {
                    /* 向内修正 */
                    {
                        servo_setangle(ROUND_LEFTANGLE);
                    }
                    motor_setforward_right(ROUND_LEFTBASE + ROUND_LEFTADD);
                    motor_setforward_left(ROUND_LEFTBASE - ROUND_LEFTSUB);
                }
                else if (TRACKER4_STATUS == t_color_black)
                {
                    /* 向外修正 */
                    {
                        servo_setangle(ROUND_RIGHTANGLE);
                    }
                    motor_setforward_right(ROUND_RIGHTBASE);
                    motor_setforward_left(ROUND_RIGHTBASE);
                }
            }
            tracking_resume();
        }
    }
    break;

    case posit_right:
    {
        for (;;)
        {
            if (ptracker_status->update == status_resloved)
            {
                continue;
            }
            /* 出口条件 */
            if (TRACKER1_STATUS == t_color_black)
            {
                break;
            }
            if (TRACKER1_STATUS == t_color_black)
            {
                round_posit = posit_left;
            }
            else if (TRACKER5_STATUS == t_color_black)
            {
                round_posit = posit_right;
            }
            tracking_resume();
        }
    }
    break;
    }
}



