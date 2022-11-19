#include "motor.h"
#include "stm32f10x.h"
#include "pwm.h"

/**
 * @brief 左轮停车
 * 
 */
void motor_setstop_left(void)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    MOTOR_PWMP_LEFT = 0;
    MOTOR_PWMN_LEFT = 0;
}

/**
 * @brief 右轮停车
 * 
 */
void motor_setstop_right(void)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    MOTOR_PWMP_RIGHT = 0;
    MOTOR_PWMN_RIGHT = 0;
}

/**
 * @brief 左轮刹车
 * 
 */
void motor_setbrake_left(void)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    MOTOR_PWMP_LEFT = 0xffff;
    MOTOR_PWMN_LEFT = 0xffff;
}

/**
 * @brief 右轮刹车
 * 
 */
void motor_setbrake_right(void)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    MOTOR_PWMP_RIGHT = 0xffff;
    MOTOR_PWMN_RIGHT = 0xffff;
}

/**
 * @brief 左轮前进
 * 
 * @param pwm_ccr_val 前进速度(PWM占空比)
 */
void motor_setforward_left(uint16_t pwm_ccr_val)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    MOTOR_PWMP_LEFT = pwm_ccr_val;
    MOTOR_PWMN_LEFT = 0;
}

/**
 * @brief 右轮前进
 * 
 * @param pwm_ccr_val 前进速度(PWM占空比)
 */
void motor_setforward_right(uint16_t pwm_ccr_val)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    MOTOR_PWMP_RIGHT = pwm_ccr_val;
    MOTOR_PWMN_RIGHT = 0;
}

/**
 * @brief 左轮后退
 * 
 * @param pwm_ccr_val 后退速度(PWM占空比)
 */
void motor_setbackward_left(uint16_t pwm_ccr_val)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    MOTOR_PWMP_LEFT = 0;
    MOTOR_PWMN_LEFT = pwm_ccr_val;
}


/**
 * @brief 右轮后退
 * 
 * @param pwm_ccr_val 后退速度(PWM占空比)
 */
void motor_setbackward_right(uint16_t pwm_ccr_val)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    MOTOR_PWMP_RIGHT = 0;
    MOTOR_PWMN_RIGHT = pwm_ccr_val;
}

/**
 * @brief 前进
 * 
 * @param pwm_val 前进速度
 */
void motor_gostright(uint16_t speed)
{
    motor_setforward_left(MOTOR_PWMBASE_LEFT + speed);
    motor_setforward_right(MOTOR_PWMBASH_RIGHT + speed);
}


