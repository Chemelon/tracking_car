#include "pid.h"

int32_t pid_integral = 0;
/**
 * @brief 位置式pid
 *
 * @param pid_this 变量句柄
 * @param target 设定值
 * @param real 测量值
 * @return int32_t 调整量
 */
#define INTEGRAL_MAX 10000
int32_t positional_pid_int(pid_type_int *pid_this, int32_t target, int32_t real)
{
	int32_t kp = pid_this->kp;
	int32_t ki = pid_this->ki;
	int32_t kd = pid_this->kd;
	static int32_t err_priv = 0;
	int32_t pid_delta, pid_err;

	pid_err = (target - real);
	pid_integral += pid_err;
	/* 积分限幅 */
	if (pid_integral > INTEGRAL_MAX)
	{
		pid_integral = INTEGRAL_MAX;
	}
	else if (pid_integral < -INTEGRAL_MAX)
	{
		pid_integral = -INTEGRAL_MAX;
	}
	pid_delta = (kp * pid_err) + (ki * pid_integral) + (kd * (pid_err - err_priv));

	err_priv = pid_err;

	return pid_delta;
}
