/*
 * dead_reckoning.cpp
 *
 *  Created on: 2019/09/01
 *      Author: User
 */

#include "dead_reckoning.hpp"

void DeadReckoning::calcurationPosition()
{
	const float x_enc_rps = 1.0f * static_cast<float>(rotenc_x_.readCount<int16_t>()) / (static_cast<float>(PPR_) * CTRL_INTERVAL_);
	const float y_enc_rps = 1.0f * static_cast<float>(rotenc_y_.readCount<int16_t>()) / (static_cast<float>(PPR_) * CTRL_INTERVAL_);

	const float x_enc_vel = M_PI * WHEEL_DIA_ * x_enc_rps;
	const float y_enc_vel = M_PI * WHEEL_DIA_ * y_enc_rps;

	rotenc_x_.clearCount();
	rotenc_y_.clearCount();

	const float d_theta = gyro_sensor_.getAngleVel() * CTRL_INTERVAL_;
	const float theta_last = gyro_sensor_.getAngle() - d_theta;

	const float x_enc_vel_remove_roll = x_enc_vel - (gyro_sensor_.getAngleVel() * ODMATER_DIST_);
	const float y_enc_vel_remove_roll = y_enc_vel - (gyro_sensor_.getAngleVel() * ODMATER_DIST_);

	const float x_vel = x_enc_vel_remove_roll * std::cos(theta_last + ANGLE_X_ENC) + y_enc_vel_remove_roll * std::cos(theta_last + ANGLE_Y_ENC);
	const float y_vel = y_enc_vel_remove_roll * std::sin(theta_last + ANGLE_Y_ENC) + x_enc_vel_remove_roll * std::sin(theta_last + ANGLE_X_ENC);

	pos_.posX(pos_.posX() + (x_vel * CTRL_INTERVAL_ ));
	pos_.posY(pos_.posY() + (y_vel * CTRL_INTERVAL_ ));
}
