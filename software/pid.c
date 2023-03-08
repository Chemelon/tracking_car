#include "pid.h"

/**
 * @brief 增量式pid
 *
 * @param pid_this 变量句柄
 * @param target 设定值
 * @param real 测量值
 * @return float 调整量
 */
float incremental_pid(pid_type *pid_this, float target, float real)
{
	float kp = pid_this->kp;
	float ki = pid_this->ki;
	float kd = pid_this->kd;
	static float err_priv1 = 0, err_priv2 = 0;
	float pid_delta,err_curr;

	err_curr = target-real;
	pid_delta = (kp * (err_curr - err_priv1)) + (ki * err_curr) + (kd * (err_curr - 2 * err_priv1 + err_priv2));

	err_priv2 = err_priv1;
	err_priv1 = err_curr;

	return pid_delta;
}

uint32_t incremental_pid_uint(pid_type_uint *pid_this, uint32_t target, uint32_t real)
{
	uint32_t kp = pid_this->kp;
	uint32_t ki = pid_this->ki;
	uint32_t kd = pid_this->kd;
	static uint32_t err_priv1 = 0, err_priv2 = 0;
	uint32_t pid_delta,err_curr;

	err_curr = target-real;
	pid_delta = (kp * (err_curr - err_priv1)) + (ki * err_curr) + (kd * (err_curr - 2 * err_priv1 + err_priv2));

	err_priv2 = err_priv1;
	err_priv1 = err_curr;

	return pid_delta;
}

/**
 * @brief 位置式pid
 *
 * @param pid_this 变量句柄
 * @param target 设定值
 * @param real 测量值
 * @return float 调整量
 */
float positional_pid(pid_type *pid_this, float target, float real)
{
	float kp = pid_this->kp;
	float ki = pid_this->ki;
	float kd = pid_this->kd;
	static float pid_integral=0;
	static float err_priv = 0;
	float pid_delta,pid_err;

	pid_err = (target-real);
	pid_integral += pid_err;

	pid_delta = (kp *pid_err)+(ki*pid_integral)+(kd*(pid_err-err_priv));

	err_priv = pid_err;

	return pid_delta;
}

uint32_t positional_pid_uint(pid_type_uint *pid_this, uint32_t target, uint32_t real)
{
	uint32_t kp = pid_this->kp;
	uint32_t ki = pid_this->ki;
	uint32_t kd = pid_this->kd;
	static uint32_t pid_integral=0;
	static uint32_t err_priv = 0;
	uint32_t pid_delta,pid_err;

	pid_err = (target-real);
	pid_integral += pid_err;

	pid_delta = (kp *pid_err)+(ki*pid_integral)+(kd*(pid_err-err_priv));

	err_priv = pid_err;

	return pid_delta;
}