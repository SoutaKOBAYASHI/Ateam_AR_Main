/*
 * systick.hpp
 *
 *  Created on: 2019/03/31
 *      Author: User
 */

#ifndef MYHEADERS_INTERRUPT_HPP_
#define MYHEADERS_INTERRUPT_HPP_

#include <functional>
#include <vector>
#include <map>

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

class SysTick_Interrupt
{
private:
	static inline std::map<SysTick_Interrupt* const,const std::function<void(void)>> callFunctions_;
	static uint32_t frequency_;
public:
	SysTick_Interrupt() = delete;
	SysTick_Interrupt(const std::function<void(void)>&& addFunc);

	static void init(uint32_t frequency, uint32_t priority)
	{
		RCC_ClocksTypeDef RCC_Clocks;
		RCC_GetClocksFreq(&RCC_Clocks);
		SysTick_Config(RCC_Clocks.HCLK_Frequency / frequency);
		NVIC_SetPriority(SysTick_IRQn, priority);
	}

	static void update(){ for(auto i : callFunctions_)i.second(); }

	inline static uint32_t readFreq(){ return frequency_; }

	virtual ~SysTick_Interrupt() { SysTick_Interrupt::callFunctions_.erase(this); }
};
#endif /* MYHEADERS_INTERRUPT_HPP_ */
