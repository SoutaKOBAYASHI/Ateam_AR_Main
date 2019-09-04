/*
 * R1370.hpp
 *
 *  Created on: 2019/09/01
 *      Author: User
 */

#ifndef R1370_HPP_
#define R1370_HPP_

#include "uart.hpp"
#include <array>

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

class R1370
{
public:
	R1370() = delete;
	R1370(UART& use_uart)
		: use_uart_(use_uart),
		  uart_interrupt_(uartName::uart1, [&](uint8_t receive_byte){ receive_data_(receive_byte);} )
	{
	}

	inline float getAngle() const { return angle_; }
	inline float getAngleVel() const { return angle_vel_; }
	inline void setAngle(const float set_val){ angle_ = set_val; };
	inline void resetAngle(){ setAngle(0.0f); };
private:
	UART& use_uart_;
	UART_Interrupt uart_interrupt_;

	float angle_ = 0.0f;
	float angle_vel_ = 0.0f;

	static constexpr float DATA_GAIN_ = 100.0f;

	void receive_data_(const uint8_t receive_byte);
};

#endif /* R1370_HPP_ */
