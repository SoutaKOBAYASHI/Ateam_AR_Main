/*
 * dead_reckoning.hpp
 *
 *  Created on: 2019/09/01
 *      Author: User
 */

#ifndef DEAD_RECKONING_HPP_
#define DEAD_RECKONING_HPP_

#include "rotary_encoder.hpp"
#include "r1370.hpp"
#include "position.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

class DeadReckoning
{
public:
	DeadReckoning() = delete;
	DeadReckoning(UART& gyro_uart_port) : gyro_sensor_(gyro_uart_port){  }

	inline float readPosX() const { return pos_.posX(); }
	inline float readPosY() const { return pos_.posY(); }
	inline float readPosTheta() { return pos_.posTheta(gyro_sensor_.getAngle()); }

	inline void setPosX(const float set_pos){ pos_.posX(set_pos); }
	inline void setPosY(const float set_pos){ pos_.posY(set_pos); }
	inline void setPosTheta(const float set_pos)
	{
		gyro_sensor_.setAngle(pos_.posTheta(set_pos));
		pos_.posTheta(set_pos);
	}
	inline void setPos(const float set_pos_x, const float set_pos_y, const float set_pos_theta)
	{
		setPosX(set_pos_x);
		setPosY(set_pos_y);
		setPosTheta(set_pos_theta);
	}

	void calcurationPosition();

	inline position<float> pos(){ return {readPosX(), readPosY(), readPosTheta()}; }
private:
	position<float> pos_ = {0.0f, 0.0f, 0.0f};

	RotaryEncoder<encoderName::RotEnc1> rotenc_x_;
	RotaryEncoder<encoderName::RotEnc2> rotenc_y_;
	R1370 gyro_sensor_;

	static constexpr uint16_t PPR_ = 8192;
	static constexpr float WHEEL_DIA_ = 50.8;
	static constexpr float ODMATER_DIST_ = 244.6;
	static constexpr float CTRL_INTERVAL_ = 0.010;

	static constexpr float ANGLE_X_ENC = 0.0f;
	static constexpr float ANGLE_Y_ENC = M_PI * -0.50f;
};

#endif /* DEAD_RECKONING_HPP_ */
