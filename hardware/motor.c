#include "motor.h"
#include "stm32f10x.h"
#include "pwm.h"

#define MOTOR_PWMP_LEFT (TIM2->CCR1)
#define MOTOR_PWMN_LEFT (TIM2->CCR2)
#define MOTOR_PWMP_RIGHT (TIM2->CCR3)
#define MOTOR_PWMN_RIGHT (TIM2->CCR4)

/* 设置TIM1 CCR4 改变占空比从而控制舵机 */
#define MOTOR_SERVO_CCR (TIM1->CCR4)

/* 舵机相关函数 */
#ifndef __SERVO_FUNC

/**
 * @brief 舵机直线方向
 *
 */
static void servo_straight(void)
{
    MOTOR_SERVO_CCR = K_SERVO_STRBASE;
}

/**
 * @brief 舵机左转
 *
 * @param angle 转动角度 在函数内换算成PWM占空比
 */
void servo_turnleft(uint16_t angle)
{
    MOTOR_SERVO_CCR = angle / K_SERVO_TURNLEFT + K_SERVO_BASELEFT;
}

/**
 * @brief 舵机右转
 *
 * @param angle 转动角度 在函数内换算成PWM占空比
 */
void servo_turnright(uint16_t angle)
{
    MOTOR_SERVO_CCR = angle / K_SERVO_TURNRIGHT + K_SERVO_BASERIGHT;
}

#endif

/* 减速电机相关函数 */
#ifndef __MOTOR_FUNC
/**
 * @brief 左轮停车
 *
 */
static void motor_setstop_left(void)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    MOTOR_PWMP_LEFT = 0;
    MOTOR_PWMN_LEFT = 0;
}

/**
 * @brief 右轮停车
 *
 */
static void motor_setstop_right(void)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    MOTOR_PWMP_RIGHT = 0;
    MOTOR_PWMN_RIGHT = 0;
}

/**
 * @brief 左轮刹车
 *
 */
static void motor_setbrake_left(void)
{
    /* 改变PWM_CCR 的值从而改变占空比 */
    MOTOR_PWMP_LEFT = 0xffff;
    MOTOR_PWMN_LEFT = 0xffff;
}

/**
 * @brief 右轮刹车
 *
 */
static void motor_setbrake_right(void)
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
static void motor_setforward_left(uint16_t pwm_ccr_val)
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
static void motor_setforward_right(uint16_t pwm_ccr_val)
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
static void motor_setbackward_left(uint16_t pwm_ccr_val)
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
static void motor_setbackward_right(uint16_t pwm_ccr_val)
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
void gostright(uint16_t speed)
{
    /* 舵机转向直线方向 */
    servo_straight();
    motor_setforward_left(MOTOR_PWMBASE_LEFT + speed);
    motor_setforward_right(MOTOR_PWMBASH_RIGHT + speed);
}

/**
 * @brief 后退
 *
 * @param speed
 */
void goback(uint16_t speed)
{
    /* 舵机转向直线方向 */
    servo_straight();
    motor_setbackward_left(MOTOR_PWMBASE_LEFT + speed);
    motor_setbackward_right(MOTOR_PWMBASH_RIGHT + speed);
}

/**
 * @brief 差速左转 通过设置左右电机不同转速实现转弯
 *
 * @param speed 转弯速度 值越大转得越快
 */
static void motor_diffal_turnleft(uint16_t speed)
{
    /* 差速转向 */
    motor_setforward_left(MOTOR_PWMBASE_LEFT + speed * K_MOTOR_TURNLEFT);
    motor_setforward_right(MOTOR_PWMBASH_RIGHT + speed);
}

/**
 * @brief 差速左转 通过设置左右电机不同转速实现转弯
 *
 * @param speed 转弯速度 值越大转得越快
 */
static void motor_diffal_turnright(uint16_t speed)
{
    /* 差速转向 */
    motor_setforward_left(MOTOR_PWMBASE_LEFT + speed);
    motor_setforward_right(MOTOR_PWMBASH_RIGHT + speed * K_MOTOR_TURNRIGHT);
}

/**
 * @brief 左转 根据实际情况决定是否使用电机差速辅助转弯
 *
 * @param angle 左转角度
 */
void turnleft(uint16_t angle)
{
    servo_turnleft(angle);
#ifdef DIFFAL_SUP
    motor_diffal_turnleft(K_DIFFAL_SUP_SPEED);
#endif
}

/**
 * @brief 右转 根据实际情况决定是否使用电机差速辅助转弯
 *
 * @param angle 右转角度
 */
void turnright(uint16_t angle)
{
    servo_turnright(angle);
#ifdef DIFFAL_SUP
    motor_diffal_turnright(K_DIFFAL_SUP_SPEED);
#endif
}
#endif
