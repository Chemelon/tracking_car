#include "tracking.h"
#include "straight.h"
#include "turning.h"
#include "SysTick.h"
#include "motor.h"
#include "curve-8.h"

void before_curve(void)
{
    /* 启动定时器 */
    TIM3->CR1 &= ~TIM_CR1_CEN;
    TIM3->CNT = 0;
    TIM3->CR1 |= TIM_CR1_CEN;
    gostraight(0);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        if (TIM3->CNT > 3800)
        {
            /* 关闭定时器 */
            TIM3->CR1 &= ~TIM_CR1_CEN;
            TIM3->CNT = 0;
            brake();
            tracking_resume();
            servo_setangle(90);
            STRAIGHT_LOG("CURVEEXIT\r\n");
#if DEBUG_CURVE
            //DEBUG_ACTIONSTOP;
#endif
            break;
        }
        if (TRACKER5_STATUS == t_color_black)
        {
            /* 向外修正*/
            servo_setangle(115);
            motor_setforward_left(STRAIGHTBASE_LEFT + 3000);
            motor_setforward_right(STRAIGHTBASE_RIGHT - 1000);
            STRAIGHT_LOG("RIGHTWARD\r\n");
        }
        // STRAIGHT_LOG("UPDATED\r\n");
        else if (TRACKER4_STATUS == t_color_black)
        {
            /* 向外修正*/
            servo_setangle(92);
            motor_setforward_left(STRAIGHTBASE_LEFT);
            motor_setforward_right(STRAIGHTBASE_RIGHT);
            STRAIGHT_LOG("RIGHTWARD\r\n");
        }
        else if (TRACKER1_STATUS == t_color_black)
        {
            /* 向内修正*/
            servo_setangle(65);
            motor_setforward_left(STRAIGHTBASE_LEFT - 1000);
            motor_setforward_right(STRAIGHTBASE_RIGHT + 3500);
            STRAIGHT_LOG("LEFTWARD\r\n");
        }
        else if (TIM3->CNT > 500 && TRACKER2_STATUS == t_color_black)
        {
            /* 向内修正*/
            servo_setangle(80);
            motor_setforward_left(STRAIGHTBASE_LEFT - 1000);
            motor_setforward_right(STRAIGHTBASE_RIGHT + 1000);
            STRAIGHT_LOG("LEFTWARD\r\n");

        }
        // STRAIGHT_LOG("STRAIGHTING\r\n");
        tracking_resume();
    }
}

void curve_out(void)
{
    /* 启动定时器 */
    TIM3->CR1 &= ~TIM_CR1_CEN;
    TIM3->CNT = 0;
    TIM3->CR1 |= TIM_CR1_CEN;
    gostraight(0);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        if (TIM3->CNT > 2000 && (TRACKER1_STATUS + TRACKER5_STATUS > 0))
        {
            /* 关闭定时器 */
            TIM3->CR1 &= ~TIM_CR1_CEN;
            TIM3->CNT = 0;
            tracking_resume();
            gostraight(0);
            brake();
            STRAIGHT_LOG("CURVEEXIT\r\n");
#if DEBUG_CURVE
            //DEBUG_ACTIONSTOP;
#endif
            break;
            
        }
        if (TRACKER3_STATUS == t_color_black || TRACKER4_STATUS == t_color_black)
        {
            /* 向内修正*/
            servo_setangle(105);
            motor_setforward_left(STRAIGHTBASE_LEFT + 3500);
            motor_setforward_right(STRAIGHTBASE_RIGHT - 1000);
            STRAIGHT_LOG("CURVE_IN\r\n");
        }
        else //if (TRACKER1_STATUS == t_color_black)
        {
            /* 向外修正*/
            servo_setangle(85);
            motor_setforward_left(STRAIGHTBASE_LEFT);
            motor_setforward_right(STRAIGHTBASE_RIGHT + 1000);
            STRAIGHT_LOG("CURVE_OUT\r\n");
        }
        // STRAIGHT_LOG("STRAIGHTING\r\n");
        tracking_resume();
    }
}

void curve_end(void)
{
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        if (TRACKER1_STATUS == t_color_black || TRACKER5_STATUS == t_color_black)
        {
            /* 关闭定时器 */
            TIM3->CR1 &= ~TIM_CR1_CEN;
            TIM3->CNT = 0;
            tracking_resume();
            brake();
            
                STRAIGHT_LOG("CURVEEXIT\r\n");
#if DEBUG_CURVE
                DEBUG_ACTIONSTOP;
#endif
                break;
            
        }
        if (TRACKER4_STATUS == t_color_black || TRACKER5_STATUS == t_color_black)
        {
            /* 向内修正*/
            servo_setangle(110);
            motor_setforward_left(STRAIGHTBASE_LEFT + 3000);
            motor_setforward_right(STRAIGHTBASE_RIGHT - 1000);
            STRAIGHT_LOG("CURVE_IN\r\n");
        }
        else if (TRACKER1_STATUS == t_color_black)
        {
            /* 向外修正*/
            servo_setangle(85);
            motor_setforward_left(STRAIGHTBASE_LEFT);
            motor_setforward_right(STRAIGHTBASE_RIGHT + 1000);
            STRAIGHT_LOG("CURVE_OUT\r\n");
        }
        tracking_resume();
    }





}


