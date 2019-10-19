/*
 * sequence.hpp
 *
 *  Created on: 2019/09/02
 *      Author: User
 */

#ifndef SEQUENCE_HPP_
#define SEQUENCE_HPP_

#include <utility>
#include <cstring>

#include "systick.hpp"
#include "auto_running.hpp"
#include "control_area_network.hpp"
#include "board_io.hpp"
#include "tape_led.hpp"
#include "led.hpp"

class Sequence
{
private:
	const bool& emergency_state_;
	const uint8_t arm_ctrl_board_add_;

	SysTick_Interrupt systick_;
	IO_sigPins<ioName::sig0, ioState::input, pinPullDirection::up> start_switch_;
	LED<LED_Color::Red> emegency_state_indicator_;
	TapeLED tape_led_;

	AutoRunning auto_running_;
	CAN_Interrupt can_interrupt_;
	ControlAreaNetwork& can_interface_;
	UART& pc_uart_port_;
	UART_Interrupt lrf_receive_interrupt;

	uint8_t received_msg_ = 0;

	inline void set_tepe_led_color_(const uint8_t r, const uint8_t g, const uint8_t b)
	{
		if(emergency_state_)tape_led_.setRGB(0,0,0);
		else tape_led_.setRGB(r,g,b);
	}

	void canReceiveMsg_(const CanRxMsg& receive_msg);
	void runSequence_();
	inline void sendCmd_(uint8_t send_cmd){ can_interface_.sendData(&send_cmd, 1, arm_ctrl_board_add_); }
	void transmitPosData_();
	void receiveLrfData(const uint8_t receive_byte);
public:
	Sequence() = delete;
	Sequence(const bool& emergency_state, const uint8_t mecha_controler_address, const AutoRunning::AutoRunningParamsStruct& auto_running_params, UART& gyro_uart_interface, UART& pc_uart_port,ControlAreaNetwork& can_interface) :
		emergency_state_(emergency_state),
		arm_ctrl_board_add_(mecha_controler_address),
		systick_( [&](){ runSequence_(); } ),
		tape_led_(0x20, can_interface),
		auto_running_(emergency_state, auto_running_params, gyro_uart_interface, can_interface),
		can_interrupt_( [&](const CanRxMsg& receive_msg){ canReceiveMsg_(receive_msg); } ),
		can_interface_(can_interface),
		pc_uart_port_(pc_uart_port),
		lrf_receive_interrupt( pc_uart_port.useUartName(), [&](const uint8_t receive_byte){ receiveLrfData(receive_byte); } ){ }

	position<float> pos(){ return { auto_running_.pos().posX(), auto_running_.pos().posY(), auto_running_.pos().posTheta()}; }
};



#endif /* SEQUENCE_HPP_ */
