/*
 * uart.cpp
 *
 *  Created on: 2019/07/27
 *      Author: SotaKobayashi
 */

#include "uart.hpp"

std::map<UART_Interrupt* const,const std::function<void(uint8_t)>> UART_Interrupt::uart1_call_functions_;
std::map<UART_Interrupt* const,const std::function<void(uint8_t)>> UART_Interrupt::uart2_call_functions_;

extern "C"
{
	void USART2_IRQHandler()
	{
		UART_Interrupt::uart1Callback();
	}

	void USART3_IRQHandler()
	{
		UART_Interrupt::uart2Callback();
	}
}
