/*
 * dead_reckoning.cpp
 *
 *  Created on: 2019/09/01
 *      Author: User
 */

#include "dead_reckoning.hpp"

void DeadReckoning::calcurationPosition()
{
	const float x_vel_local = (WHEEL_DIA_ * static_cast<float>(M_PI) * (rotenc_x_.readCount<float>() / static_cast<float>(PPR_))) + CTRL_INTERVAL_ * gyro_sensor_.getAngleVel() * ODOMATER_DIST_;
	const float y_vel_local = (WHEEL_DIA_ * static_cast<float>(M_PI) * (rotenc_y_.readCount<float>() / static_cast<float>(PPR_))) - CTRL_INTERVAL_ * gyro_sensor_.getAngleVel() * ODOMATER_DIST_;
	rotenc_x_.clearCount();
	rotenc_y_.clearCount();

	pos_.posX(pos_.posX() + (-1 * x_vel_local * std::cos(gyro_sensor_.getAngle())) - (y_vel_local * std::sin(gyro_sensor_.getAngle())));
	pos_.posY(pos_.posY() + (-1 * x_vel_local * std::sin(gyro_sensor_.getAngle())) + (y_vel_local * std::cos(gyro_sensor_.getAngle())));
}
