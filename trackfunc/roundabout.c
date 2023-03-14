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
        if ((TRACKER1_STATUS == t_color_black) || (TRACKER5_STATUS == t_color_black))
        {
            /* 根据实际数据修改的条件 OK */
            if (TRACKER3_STATUS == t_color_black)
            {
                servo_setangle(90);
                tracking_resume();
                brake();
                break;
            }
        }
        if (TRACKER4_STATUS == t_color_black)
        {
            /* 偏左 向右修正*/
            servo_setangle(100);
            motor_setforward_left(STRAIGHTBASE_LEFT + 2000);
            motor_setforward_right(STRAIGHTBASE_RIGHT);
        }
        else if (TRACKER2_STATUS == t_color_black)
        {
            /* 偏右 向左修正*/
            servo_setangle(80);
            motor_setforward_left(STRAIGHTBASE_LEFT);
            motor_setforward_right(STRAIGHTBASE_RIGHT + 2000);
        }
        tracking_resume();
    }
}


/* 进环 */
void circle_in(roundabout_posit_type round_posit)
{
    // TIM3->CR1 &= ~TIM_CR1_CEN;
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
            if (TRACKER5_STATUS == t_color_black && (TIM3->CNT > 1500))
            {
                DEBUG_ACTIONSTOP;
                break;
            }
            if (TRACKER2_STATUS == t_color_black || TRACKER3_STATUS == t_color_black)
            {
                /* 向内修正(左) */
                if ((TIM3->CNT < 1500))
                {
                    servo_setangle(80);
                }
                else
                {
                    servo_setangle(ROUND_LEFTANGLE);
                }
                motor_setforward_right(ROUND_LEFTBASE + ROUND_LEFTADD);
                motor_setforward_left(ROUND_LEFTBASE - ROUND_LEFTSUB);
            }
            else if (TRACKER4_STATUS == t_color_black)
            {
                /* 向外修正 */
                if ((TIM3->CNT < 1500))
                {
                    servo_setangle(100);
                }
                else
                {
                    servo_setangle(ROUND_RIGHTANGLE);
                }
                motor_setforward_right(ROUND_RIGHTBASE + ROUND_RIGHTADD);
                motor_setforward_left(ROUND_RIGHTBASE - ROUND_RIGHTSUB);
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

/* 出环 */
void circle_out(void)
{
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        if (1 /* 出口条件 */)
        {
        }
        tracking_resume();
    }
}

/* 左转环岛 */
void roundabout_left(void)
{
    servo_setangle(90);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        if (1 /* 出口条件 */)
        {
            ROUNDABOUT_LOG("ROUNDABOUTEXIT\r\n");
            tracking_resume();
            servo_setangle(90);
            brake();
#if DEBUG_ROUNDABOUT
            DEBUG_ACTIONSTOP;
#endif
            break;
        }
        if (1)
        {
        }
        else if (1)
        {
        }

        tracking_resume();
    }
}

/* 右转环岛 */
void roundabout_right(void)
{
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        if (1 /* 出口条件 */)
        {
        }
        tracking_resume();
    }
}