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
private:
	static constexpr int32_t ERROR_ = 150;
	static constexpr float ROLLING_SPEED_ = 100.0f;

	static constexpr pid_gain<float> ROLLING_GAIN_ = {150.0f, 0.00f, 2.0f};
	static constexpr pid_gain<float> RUNNING_GAIN_ = {300.0f, 10.0f, 1.0f};
public:
	OmniWheel4s omni_wheel_;
	DeadReckoning dead_reckoning_;

	AutoRunning() = delete;
	AutoRunning(UART& gyro_uart_interface, ControlAreaNetwork& can_interface)
		: omni_wheel_(can_interface), dead_reckoning_(gyro_uart_interface) { }

	int pointControl(const uint8_t route_num);
	inline void deadReckoningUpdate(){ dead_reckoning_.calcurationPosition(); }

	position<float> pos(){ return { dead_reckoning_.readPosX(), dead_reckoning_.readPosY(), dead_reckoning_.readPosTheta()}; }
};

#endif /* AUTO_RUNNING_HPP_ */
