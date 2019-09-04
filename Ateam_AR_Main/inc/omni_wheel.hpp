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

extern int32_t wheelspeed;
extern int32_t runvector;

class OmniWheel4s
{
public:
	OmniWheel4s() = delete;
	OmniWheel4s(ControlAreaNetwork& use_can_interface)
		: wheel1(0x10, WHEEL1_PARAM_, use_can_interface),
		  wheel2(0x11, WHEEL2_PARAM_, use_can_interface),
		  wheel3(0x12, WHEEL3_PARAM_, use_can_interface),
		  wheel4(0x13, WHEEL4_PARAM_, use_can_interface),
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
	void setRunningVector(const float vel, const float theta, const int32_t rolling_vel = 0)
	{
		const float vec1 = vel * std::cos(theta + M_PI * 0.25f);
		const float vec2 = vel * std::sin(theta + M_PI * 0.25f);

		constexpr float VEL_TO_RPM_GAIN = 60 / WHEEL_DIA_;

		runvector = -vec1 * VEL_TO_RPM_GAIN;
		wheelspeed = rolling_vel + static_cast<int32_t>(-vec1 * VEL_TO_RPM_GAIN);

		wheel1.setSpeed(rolling_vel + static_cast<int32_t>(vec2 * VEL_TO_RPM_GAIN), MotorDriver::MotorDriveMode::PID);
		wheel2.setSpeed(rolling_vel + static_cast<int32_t>(-vec1 * VEL_TO_RPM_GAIN), MotorDriver::MotorDriveMode::PID);
		wheel3.setSpeed(rolling_vel + static_cast<int32_t>(-vec2 * VEL_TO_RPM_GAIN), MotorDriver::MotorDriveMode::PID);
		wheel4.setSpeed(rolling_vel + static_cast<int32_t>(vec1 * VEL_TO_RPM_GAIN), MotorDriver::MotorDriveMode::PID);
	}
private:
	MotorDriver wheel1;
	MotorDriver wheel2;
	MotorDriver wheel3;
	MotorDriver wheel4;

	SysTick_Interrupt systick_;

	static constexpr MotorDriver::SendParamsType WHEEL1_PARAM_ = {-0.017f, -0.005f, -0.0007f, 5000, 8192};
	static constexpr MotorDriver::SendParamsType WHEEL2_PARAM_ = {-0.017f, -0.005f, -0.0007f, 5000, 8192};
	static constexpr MotorDriver::SendParamsType WHEEL3_PARAM_ = {-0.017f, -0.005f, -0.0007f, 5000, 8192};
	static constexpr MotorDriver::SendParamsType WHEEL4_PARAM_ = {-0.017f, -0.005f, -0.0007f, 5000, 8192};

	inline void systick_driver_update_()
	{
		wheel1.update();
		wheel2.update();
		wheel3.update();
		wheel4.update();
	}

	static constexpr float WHEEL_DIA_ = 101.6f;
};


#endif /* OMNI_WHEEL_HPP_ */
