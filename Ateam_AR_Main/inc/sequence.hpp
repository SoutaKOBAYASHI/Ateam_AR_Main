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
#include "control_area_network.hpp"
#include "board_io.hpp"

class Sequence
{
private:
	static constexpr uint8_t arm_ctrl_board_add_ = 0x05;

	SysTick_Interrupt systick_;
	IO_sigPins<ioName::sig0, ioState::input, pinPullDirection::up> start_switch_;

	AutoRunning auto_running_;
	CAN_Interrupt can_interrupt_;
	ControlAreaNetwork can_interface_;

	uint8_t received_msg_ = 0;

	void canReceiveMsg(const CanRxMsg& receive_msg);
	void runSequence();
	inline void sendCmd(uint8_t send_cmd){ can_interface_.sendData(&send_cmd, 1, arm_ctrl_board_add_); }

public:
	Sequence() = delete;
	Sequence(const AutoRunning::AutoRunningParamsStruct& auto_running_params, UART& gyro_uart_interface, ControlAreaNetwork& can_interface) :
		systick_( [&](){ runSequence(); } ),
		auto_running_(auto_running_params, gyro_uart_interface, can_interface),
		can_interrupt_( [&](const CanRxMsg& receive_msg){ canReceiveMsg(receive_msg); } ),
		can_interface_(can_interface){ }

	position<float> pos(){ return { auto_running_.pos().posX(), auto_running_.pos().posY(), auto_running_.pos().posTheta()}; }
};



#endif /* SEQUENCE_HPP_ */
