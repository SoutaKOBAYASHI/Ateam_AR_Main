/*
 * sbdbt.hpp
 *
 *  Created on: 2019/10/09
 *      Author: SotaKobayashi
 */

#ifndef SBDBT_HPP_
#define SBDBT_HPP_

#include <array>

#include "systick.hpp"
#include "uart.hpp"
#include "vector2d.hpp"
#include "led.hpp"
class SbdbtDualShock
{
public:
	enum class Buttons : uint16_t
	{
		UP		= 0x0001,
		DOWN	= 0x0002,
		RIGHT	= 0x0004,
		LEFT	= 0x0008,
		TRIANGLE= 0x0010,
		CROSS	= 0x0020,
		CIRCLE	= 0x0040,
		START	= 0x0080,
		SQUARE	= 0x0100,
		L1		= 0x0200,
		L2		= 0x0400,
		R1		= 0x0800,
		R2		= 0x1000,
		SELECT	= 0x2000
	};

	enum class Pad { RIGHT, LEFT };
	SbdbtDualShock() = delete;
	SbdbtDualShock(const uint32_t timeout_time_ms, UART& use_uart_interface) :
		timeout_time_ms_(timeout_time_ms),
		use_uart_interface_(use_uart_interface),
		uart_interrupt_(use_uart_interface.useUartName(), [&](uint8_t received_byte){ receiveByte(received_byte); }),
		systick_([&](){ incrementTimeoutCount(); }){}

	inline bool isButtonPushed(const Buttons check_button_name)
	{
		const uint16_t check_buttons_bit = static_cast<uint16_t>(check_button_name);
		return ((redeButtonsByte() & check_buttons_bit) == check_buttons_bit);
	}

	vector2d<float> readAnalogPad(const Pad read_pad) const;
private:
	const uint32_t timeout_time_ms_;
	UART& use_uart_interface_;
	UART_Interrupt uart_interrupt_;
	SysTick_Interrupt systick_;

	float now_timeout_count_ = 0.0f;
	std::array<uint8_t, 8> received_data_arr_ = {0x80, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x00};

	inline uint16_t redeButtonsByte() { return (static_cast<uint16_t>(received_data_arr_.at(1)) << 8 | static_cast<uint16_t>(received_data_arr_.at(2))); }

	void receiveByte(const uint8_t receive_byte);

	inline void incrementTimeoutCount()
	{
		constexpr float GAIN_SEC_TO_MS = 1000.0;
		now_timeout_count_ += GAIN_SEC_TO_MS / static_cast<float>(SysTick_Interrupt::readFreq());
	}
	LED<LED_Color::Red> led_;
};



#endif /* SBDBT_HPP_ */
