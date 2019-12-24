/*
 * omni_wheel.hpp
 *
 *  Created on: 2019/09/01
 *      Author: User
 */

#ifndef OMNI_WHEEL_HPP_
#define OMNI_WHEEL_HPP_

#include <functional>
#include <cmath>

#include "motor_driver.hpp"
#include "control_area_network.hpp"
#include "systick.hpp"

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

class OmniWheel4s
{
public:
	struct OmniWheel4s_ParamStruct{
		MotorDriver::SendParamsType wheel1_param;
		MotorDriver::SendParamsType wheel2_param;
		MotorDriver::SendParamsType wheel3_param;
		MotorDriver::SendParamsType wheel4_param;

		uint8_t wheel1_address;
		uint8_t wheel2_address;
		uint8_t wheel3_address;
		uint8_t wheel4_address;

		float wheel_dia;
		bool direction;
		float angle_offset;
		float wheel_dist;
	};

	OmniWheel4s() = delete;
	OmniWheel4s(const bool& emergency_state, const OmniWheel4s_ParamStruct& omni_params, ControlAreaNetwork& use_can_interface)
		: params_(omni_params),
		  wheel1(emergency_state, omni_params.wheel1_address, omni_params.wheel1_param, use_can_interface),
		  wheel2(emergency_state, omni_params.wheel2_address, omni_params.wheel2_param, use_can_interface),
		  wheel3(emergency_state, omni_params.wheel3_address, omni_params.wheel3_param, use_can_interface),
		  wheel4(emergency_state, omni_params.wheel4_address, omni_params.wheel4_param, use_can_interface),
		  systick_( [&](){ systick_driver_update_(); } ){}

	inline void setWheelSpeed(const uint8_t wheel_number, const int32_t set_speed)
	{
		switch(wheel_number)
		{
		case 1:
			wheel1.setSpeed(set_speed, MotorDriver::MotorDriveMode::DutyControl);
			break;
		case 2:
			wheel2.setSpeed(set_speed);
			break;
		case 3:
			wheel3.setSpeed(set_speed);
			break;
		case 4:
			wheel4.setSpeed(set_speed);
			break;
		}
	}
	inline void setRunningVector(const float vel, const float theta,  const float angular_vel)
	{
		const float theta_calibrated = (theta - params_.angle_offset) * ((params_.direction == true) ? 1.0f : -1.0f);
		const float vec1 = vel * std::cos(theta_calibrated);
		const float vec2 = vel * std::sin(theta_calibrated);

		const float VEL_TO_RPM_GAIN = 60.0f / (params_.wheel_dia * 2.0f * M_PI);
		const int32_t angular_vel_rpm = static_cast<int32_t>(60.0f * angular_vel * params_.wheel_dist / (params_.wheel_dia * M_PI) * ((params_.direction == true) ? 1.0f : -1.0f));

		wheel1.setSpeed(angular_vel_rpm + static_cast<int32_t>(vec2 * VEL_TO_RPM_GAIN), MotorDriver::MotorDriveMode::PID);
		wheel2.setSpeed(angular_vel_rpm + static_cast<int32_t>(-vec1 * VEL_TO_RPM_GAIN), MotorDriver::MotorDriveMode::PID);
		wheel3.setSpeed(angular_vel_rpm + static_cast<int32_t>(-vec2 * VEL_TO_RPM_GAIN), MotorDriver::MotorDriveMode::PID);
		wheel4.setSpeed(angular_vel_rpm + static_cast<int32_t>(vec1 * VEL_TO_RPM_GAIN), MotorDriver::MotorDriveMode::PID);
	}
private:
	const OmniWheel4s_ParamStruct params_;

	MotorDriver wheel1;
	MotorDriver wheel2;
	MotorDriver wheel3;
	MotorDriver wheel4;

	SysTick_Interrupt systick_;

	inline void systick_driver_update_()
	{
		wheel1.update();
		wheel2.update();
		wheel3.update();
		wheel4.update();
	}
};


#endif /* OMNI_WHEEL_HPP_ */
