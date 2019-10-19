/*
 * tape_led.hpp
 *
 *  Created on: 2019/10/15
 *      Author: SotaKobayashi
 */

#ifndef TAPE_LED_HPP_
#define TAPE_LED_HPP_

#include "control_area_network.hpp"

class TapeLED
{
	const uint8_t controler_address_;
	ControlAreaNetwork& can_interface_;
public:
	TapeLED() = delete;
	TapeLED(const uint8_t controler_address, ControlAreaNetwork& can_interface) :
		controler_address_(controler_address),
		can_interface_(can_interface)
	{
	}

	inline void setRGB(const uint8_t r_val, const uint8_t g_val, const uint8_t b_val)
	{
		const std::array<uint8_t, 3> transmit_data = { r_val, g_val, b_val };
		can_interface_.sendData(transmit_data, controler_address_);
	}
};



#endif /* TAPE_LED_HPP_ */
