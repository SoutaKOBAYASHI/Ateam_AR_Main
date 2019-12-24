/*
 * auto_running.hpp
 *
 *  Created on: 2019/09/01
 *      Author: User
 */

#ifndef AUTO_RUNNING_HPP_
#define AUTO_RUNNING_HPP_

#include "route.hpp"
#include "dead_reckoning.hpp"
#include "omni_wheel.hpp"
#include "vector2d.hpp"
#include "pid.hpp"

class AutoRunning
{
public:
	struct AutoRunningParamsStruct
	{
		OmniWheel4s::OmniWheel4s_ParamStruct omni_params;
		int32_t acceptable_error;
		int32_t acceptable_error_endpoint;
		float rolling_speed;
		pid_gain<float> rolling_gain;
		pid_gain<float> running_gain;
	};

	OmniWheel4s omni_wheel_;
	DeadReckoning dead_reckoning_;

	AutoRunning() = delete;
	AutoRunning(const bool& emergency_state, const AutoRunningParamsStruct& auto_running_params, UART& gyro_uart_interface, ControlAreaNetwork& can_interface)
		: omni_wheel_(emergency_state, auto_running_params.omni_params, can_interface),
		  dead_reckoning_(gyro_uart_interface),
		  auto_running_params_(auto_running_params){ }

	int pointControl(const uint8_t route_num);
	inline void deadReckoningUpdate(){ dead_reckoning_.calcurationPosition(); }

	position<float> pos(){ return { dead_reckoning_.readPosX(), dead_reckoning_.readPosY(), dead_reckoning_.readPosTheta()}; }

	virtual ~AutoRunning() {}

private:
	const AutoRunningParamsStruct auto_running_params_;
};

#endif /* AUTO_RUNNING_HPP_ */
