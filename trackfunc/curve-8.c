#include "tracking.h"
#include "straight.h"
#include "turning.h"
#include "SysTick.h"
#include "motor.h"
#include "curve-8.h"

/* 出口条件不同的直线循迹函数 */
void before_curve(void)
{

    servo_setangle(90);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        if (((TRACKER1_STATUS == t_color_black) && (TRACKER5_STATUS == t_color_black)) &&
            ((TRACKER2_STATUS + TRACKER3_STATUS + TRACKER4_STATUS) < 3))
        {
            brake();
            tracking_resume();
            servo_setangle(90);
            STRAIGHT_LOG("STRAITHTEXIT\r\n");
#if DEBUG_STRAIGHT
            DEBUG_ACTIONSTOP;
#endif
            break;
        }
        // STRAIGHT_LOG("UPDATED\r\n");
        if (TRACKER4_STATUS == t_color_black)
        {
            /* 偏左 向右修正*/
            servo_setangle(95);
            motor_setforward_left(STRAIGHTBASE_LEFT + 2000);
            motor_setforward_right(STRAIGHTBASE_RIGHT);
            STRAIGHT_LOG("RIGHTWARD\r\n");
        }
        else if (TRACKER2_STATUS == t_color_black)
        {
            /* 偏右 向左修正*/
            servo_setangle(85);
            motor_setforward_left(STRAIGHTBASE_LEFT);
            motor_setforward_right(STRAIGHTBASE_RIGHT + 2000);
            STRAIGHT_LOG("LEFTWARD\r\n");
        }
        STRAIGHT_LOG("STRAIGHTING\r\n");
        tracking_resume();
    }
}
