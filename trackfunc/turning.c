#include "tracking.h"
#include "turning.h"
#include "SysTick.h"
#include "motor.h"

/* 巡线90度左转 */
void tracking_left(void)
{
    TRACKLEFT90_LOG("LEFT90IN");
    /* 向左大转弯 右轮加速左轮减速 */
    servo_setangle(S_LEFT90);
    motor_setforward_right(RIGHTTURNBASE_RIGHT + LEFTTURN_ADD);
    motor_setbrake_left();
    motor_setbackward_left(LEFTTURNBASE_LEFT + 3000);
    Delay_ms(25);
    motor_setbrake_left();
    motor_setforward_right(RIGHTTURNBASE_RIGHT);
    motor_setforward_left(LEFTTURNBASE_LEFT + 3000);
    Delay_ms(55);

    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            // TRACKLEFT90_LOG("LEFT90TURNING\r\n");
            continue;
        }
        //tracker_sendinfo();
        /* 中心和靠外圈的光电均为黑色退出转弯模式 */
        // if (TRACKER3_STATUS == t_color_black && TRACKER2_STATUS == t_color_black)
        if (TRACKER2_STATUS == t_color_black && TRACKER5_STATUS == t_color_white &&  TRACKER1_STATUS== t_color_white)
        {
            servo_setangle(110);
            brake();
            tracking_resume();
            TRACKLEFT90_LOG("LEFT90EXIT");
#if DEBUG_TRACKLEFT90
            DEBUG_ACTIONSTOP;
#endif
            break;
        }
        tracking_resume();
    }
}

/* 巡线90度右转 */
void tracking_right(void)
{
    TRACKRIGHT90_LOG("RIGHT90IN");
    /* 向右大转弯 左轮加速右轮减速 */
    servo_setangle(S_RIGHT90);
    motor_setforward_left(RIGHTTURNBASE_LEFT + RIGHTTURN_ADD);
    motor_setbrake_right();
    motor_setbackward_right(RIGHTTURNBASE_RIGHT + 3000);
    Delay_ms(28);
    motor_setforward_left(RIGHTTURNBASE_LEFT);
    motor_setbrake_right();
    motor_setforward_right(RIGHTTURNBASE_RIGHT + 3000);
    Delay_ms(55);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        /* 中心和靠外圈的光电均为黑色退出转弯模式 */
        if (TRACKER4_STATUS == t_color_black && TRACKER1_STATUS == t_color_white && TRACKER5_STATUS == t_color_white)
        {
            servo_setangle(65);
            motor_setforward_right(RIGHTTURNBASE_RIGHT);
            motor_setforward_left(RIGHTTURNBASE_LEFT);
            brake();
            tracking_resume();
            TRACKRIGHT90_LOG("RIGHT90EXIT");
#if DEBUG_TRACKRIGHT90
            DEBUG_ACTIONSTOP;
#endif
            break;
        }
        tracking_resume();
    }
}
