/*
 * autorunning_params.hpp
 *
 *  Created on: 2019/10/10
 *      Author: SotaKobayashi
 */

#ifndef AUTORUNNING_PARAMS_HPP_
#define AUTORUNNING_PARAMS_HPP_

#include "auto_running.hpp"

constexpr MotorDriver::SendParamsType MOTOR1_PARAMS	= {-0.017f, -0.005f, -0.0007f, 5000, 8192};
constexpr MotorDriver::SendParamsType MOTOR2_PARAMS	= {-0.017f, -0.005f, -0.0007f, 5000, 8192};
constexpr MotorDriver::SendParamsType MOTOR3_PARAMS	= {-0.017f, -0.005f, -0.0007f, 5000, 8192};
constexpr MotorDriver::SendParamsType MOTOR4_PARAMS	= {-0.017f, -0.005f, -0.0007f, 5000, 8192};
constexpr float WHEEL_DIA							= 101.6f;

constexpr int32_t AUTO_RUNNING_ERROR	= 150;
constexpr float ROLLING_SPEED			= 100.0f;
constexpr pid_gain<float> ROLLING_GAIN	= {150.0f, 0.00f, 2.0f};
constexpr pid_gain<float> RUNNING_GAIN	= {300.0f, 10.0f, 1.0f};


constexpr OmniWheel4s::OmniWheel4s_ParamStruct OMNI_PARAMS =
{
	MOTOR1_PARAMS,
	MOTOR2_PARAMS,
	MOTOR3_PARAMS,
	MOTOR4_PARAMS,
	WHEEL_DIA
};

constexpr AutoRunning::AutoRunningParamsStruct AUTO_RUNNING_PARAMS =
{
	OMNI_PARAMS,
	AUTO_RUNNING_ERROR,
	ROLLING_SPEED,
	ROLLING_GAIN,
	RUNNING_GAIN
};

#endif /* AUTORUNNING_PARAMS_HPP_ */
