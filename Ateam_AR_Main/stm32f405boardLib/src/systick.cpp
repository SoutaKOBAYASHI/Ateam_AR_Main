/*
 * intrrupt.cpp
 *
 *  Created on: 2019/04/06
 *      Author: User
 */

#include <systick.hpp>

uint32_t SysTick_Interrupt::frequency_ = 0;

SysTick_Interrupt::SysTick_Interrupt(const std::function<void(void)>&& addFunc)
{
	callFunctions_.insert(std::make_pair(this, addFunc));
}

extern"C"
{
	void SysTick_Handler(void)
	{
		SysTick_Interrupt::update();
	}
}
