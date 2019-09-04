/*
 * sequence.hpp
 *
 *  Created on: 2019/09/02
 *      Author: User
 */

#ifndef SEQUENCE_HPP_
#define SEQUENCE_HPP_

#include "systick.hpp"
#include "auto_running.hpp"

#include "board_io.hpp"

class Sequence
{
private:
	SysTick_Interrupt systick_;
	IO_sigPins<ioName::sig0, ioState::input, pinPullDirection::up> start_switch_;

	AutoRunning auto_running_;

	void runSequence();

public:
	Sequence() = delete;
	Sequence(UART& gyro_uart_interface, ControlAreaNetwork& can_interface) :
		systick_( [&](){ runSequence(); } ),
		auto_running_(gyro_uart_interface, can_interface){ }

	position<float> pos(){ return { auto_running_.pos().posX(), auto_running_.pos().posY(), auto_running_.pos().posTheta()}; }
};



#endif /* SEQUENCE_HPP_ */
