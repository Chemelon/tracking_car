#include "pid.h"

pid_type_uint tracker_pid = 
{
    10,
    10,
    10,
};

/**
 * @brief 位置式pid
 *
 * @param pid_this 变量句柄
 * @param target 设定值
 * @param real 测量值
 * @return uint32_t 调整量
 */
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