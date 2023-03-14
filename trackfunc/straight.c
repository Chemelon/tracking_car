#include "tracking.h"
#include "straight.h"
#include "motor.h"
#include "pid.h"
#include "math.h"

/* 直线循迹 */
#define EXIT_COND (((TRACKER1_STATUS == t_color_black) || (TRACKER5_STATUS == t_color_black)) && \
                   ((TRACKER2_STATUS + TRACKER3_STATUS + TRACKER4_STATUS) < 2))
void tracking_straight(void)
{
    servo_setangle(S_STRAIGHTWARD);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }
        if (EXIT_COND)
        {
#undef EXIT_COND
            servo_setangle(S_STRAIGHTWARD);
            STRAIGHT_LOG("STRAITHTEXIT\r\n");
            tracking_resume();
            brake();
#if DEBUG_STRAIGHT
            DEBUG_ACTIONSTOP;
#endif
            break;
        }
        // STRAIGHT_LOG("UPDATED\r\n");
        if (TRACKER4_STATUS == t_color_black)
        {
            /* 偏左 向右修正*/
            servo_setangle(S_RIGHTWARD);
            motor_setforward_left(STRAIGHTBASE_LEFT + RIGHTWARD_ADD);
            motor_setforward_right(STRAIGHTBASE_RIGHT);
            // tracker_sendinfo();
            STRAIGHT_LOG("RIGHTWARD\r\n");
        }
        else if (TRACKER2_STATUS == t_color_black)
        {
            /* 偏右 向左修正*/
            servo_setangle(S_LEFTWARD);
            motor_setforward_left(STRAIGHTBASE_LEFT);
            motor_setforward_right(STRAIGHTBASE_RIGHT + LEFTWARD_ADD);
            // tracker_sendinfo();
            STRAIGHT_LOG("LEFTWARD\r\n");
        }
        STRAIGHT_LOG("STRAIGHTING\r\n");
        tracking_resume();
    }
}

/* 直线循迹pid */
#define EXIT_COND (TRACKER2_STATUS == t_color_black && TRACKER3_STATUS == t_color_black && TRACKER4_STATUS == t_color_black)
#define DELTA_MAX 10000
// 往右偏为正
void tracking_straight_pid(void)
{
    pid_type_int straight_pid = {200, 1, 0};
    int32_t delta = 0, angle = 0;
    servo_setangle(S_STRAIGHTWARD);
    for (;;)
    {
        if (ptracker_status->update == status_resloved)
        {
            continue;
        }

        if (EXIT_COND)
        {
            stop();
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
//        if (delta > 0)
        {
            // 右偏
            servo_setangle(90);
            motor_setforward_left(STRAIGHTBASE_LEFT - delta);
            motor_setforward_right(STRAIGHTBASE_RIGHT + delta);
        }
//        else
//        {
//            // 左偏
//            servo_setangle(90 + angle);
//            motor_setforward_left(STRAIGHTBASE_LEFT - delta);
//            motor_setforward_right(STRAIGHTBASE_RIGHT + delta);
//        }
        tracker_sendinfo();
        printf("%d %d\r\n", delta, angle);
        tracking_resume();
    }
}
