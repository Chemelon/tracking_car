#include "motor.h"
#include "stm32f10x.h"
#include "pwm.h"
/* 舵机相关函数 */
/**
 * @brief 调节占空比
 * 
 * @param CCR_value 占空比(CCR)值(0~20000) 每单位1us
 */
void servo_set_dutyclcle(uint16_t CCR_value)
{
    S_PWM_CCR = CCR_value;
}

/**
 * @brief 将角度换算为占空比
 * 
 * @param angle 角度
 */
void servo_setangle(uint16_t angle)
{
    S_PWM_CCR = (uint16_t)((angle / 180.0f) * S_PWM_CCR_180 + S_PWM_CCR_0);
}

/**
 * @brief 左轮停车
 *
 */
void motor_setstop_left(void)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    M_PWM_LEFT_P = 0;
    M_PWM_LEFT_N = 0;
}

/**
 * @brief 右轮停车
 *
 */
static void motor_setstop_right(void)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    M_PWM_RIGHT_P = 0;
    M_PWM_RIGHT_N = 0;
}

/**
 * @brief 左轮刹车
 *
 */
void motor_setbrake_left(void)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    M_PWM_LEFT_P = 0xffff;
    M_PWM_LEFT_N = 0xffff;
}

/**
 * @brief 右轮刹车
 *
 */
void motor_setbrake_right(void)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    M_PWM_RIGHT_P = 0xffff;
    M_PWM_RIGHT_N = 0xffff;
}

/**
 * @brief 左轮后退
 *
 * @param pwm_ccr_val 后退速度(PWM占空比)
 */
void motor_setbackward_left(uint16_t pwm_ccr_val)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    M_PWM_LEFT_P = pwm_ccr_val;
    M_PWM_LEFT_N = 0;
}

/**
 * @brief 右轮后退
 *
 * @param pwm_ccr_val 后退速度(PWM占空比)
 */
void motor_setbackward_right(uint16_t pwm_ccr_val)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    M_PWM_RIGHT_P = pwm_ccr_val;
    M_PWM_RIGHT_N = 0;
}

/**
 * @brief 左轮前进
 *
 * @param pwm_ccr_val 前进速度(PWM占空比)
 */
void motor_setforward_left(uint16_t pwm_ccr_val)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    M_PWM_LEFT_P = 0;
    M_PWM_LEFT_N = pwm_ccr_val;
}

/**
 * @brief 右轮前进
 *
 * @param pwm_ccr_val 前进速度(PWM占空比)
 */
void motor_setforward_right(uint16_t pwm_ccr_val)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    M_PWM_RIGHT_P = 0;
    M_PWM_RIGHT_N = pwm_ccr_val;
}

/* ************************************************ */

/* ************************************************ */

/**
 * @brief 后退
 *
 * @param pwm_val 速度
 */
void goback(uint16_t speed)
{
    /* 新车和原车正好相反 */
    motor_setforward_left(STRAIGHTBASE_LEFT + speed);
    motor_setforward_right(STRAIGHTBASE_RIGHT + speed);
}

/**
 * @brief 前进
 *
 * @param speed
 */
void gostraight(uint16_t speed)
{
    motor_setforward_left(STRAIGHTBASE_LEFT + speed);
    motor_setforward_right(STRAIGHTBASE_RIGHT + speed);
}

/**
 * @brief 刹车
 *
 */
void brake(void)
{
    motor_setbrake_left();
    motor_setbrake_right();
}

/**
 * @brief 停车
 *
 */
void stop(void)
{
    motor_setstop_right();
    motor_setstop_left();
}
