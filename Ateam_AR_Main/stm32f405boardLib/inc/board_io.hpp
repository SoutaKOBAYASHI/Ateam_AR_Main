/*
 * signal.hpp
 *
 *  Created on: 2019/04/04
 *      Author: User
 */

#ifndef MYHEADERS_BOARD_IO_HPP_
#define MYHEADERS_BOARD_IO_HPP_

#include <type_traits>

#include "stm32f4xx.h"

enum class ioName : uint16_t
{
	sig0	= (0x0001 << 0),
	sig1	= (0x0001 << 1),
	sig2	= (0x0001 << 2),
	sig3	= (0x0001 << 3),
	sig4	= (0x0001 << 4),
	sig5	= (0x0001 << 5),
	sig6	= (0x0001 << 6),
	sig7	= (0x0001 << 7)
};
enum class ioState { input, output };
enum class pinPullDirection{ no, up, down};
enum class pinOutputMode{ pushPull, openDrain};

template
<ioName useIOname, ioState setIOstate, pinPullDirection setPinPuPd = pinPullDirection::no, pinOutputMode setPinOType = pinOutputMode::pushPull>
class IO_sigPins
{
public:
	IO_sigPins(){ GPIO_pinsInit(); }

	inline bool readNowState()
	{
		return GPIO_ReadInputDataBit(usePort_, (uint16_t)useIOname);
	}

	inline void setNewState(const bool isPinHigh)
	{
		if(isPinHigh)	GPIO_SetBits(usePort_, (uint16_t)useIOname);
		else			GPIO_ResetBits(usePort_, (uint16_t)useIOname);
	}
	inline void pinToggle(){ GPIO_ToggleBits(usePort_, (uint16_t)useIOname); }

private:
	void GPIO_pinsInit()
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

		constexpr GPIO_InitTypeDef GPIO_insitStruct
		{
			/*GPIO pin*/
			(uint16_t)useIOname,

			/*GPIO Mode*/
			setIOstate == ioState::input ? GPIO_Mode_IN : GPIO_Mode_OUT,

			/*GPIO Speed*/
			GPIO_Speed_100MHz,

			/*GPIO_OType*/
			setPinOType == pinOutputMode::pushPull ? GPIO_OType_PP : GPIO_OType_OD,

			/*GPIO_PuPd*/
			setPinPuPd == pinPullDirection::no ? GPIO_PuPd_NOPULL	:
			setPinPuPd == pinPullDirection::up ? GPIO_PuPd_UP		: GPIO_PuPd_DOWN
		};
		GPIO_Init_reference(usePort_, GPIO_insitStruct);
	}

protected:
	GPIO_TypeDef* const usePort_ = GPIOC;

	static void GPIO_Init_reference(GPIO_TypeDef* GPIOx, const GPIO_InitTypeDef& GPIO_InitStruct)
	{
	  uint32_t pinpos = 0x00, pos = 0x00 , currentpin = 0x00;

	  /* Check the parameters */
	  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	  assert_param(IS_GPIO_PIN(GPIO_InitStruct.GPIO_Pin));
	  assert_param(IS_GPIO_MODE(GPIO_InitStruct.GPIO_Mode));
	  assert_param(IS_GPIO_PUPD(GPIO_InitStruct.GPIO_PuPd));

	  /* ------------------------- Configure the port pins ---------------- */
	  /*-- GPIO Mode Configuration --*/
	  for (pinpos = 0x00; pinpos < 0x10; pinpos++)
	  {
	    pos = ((uint32_t)0x01) << pinpos;
	    /* Get the port pins position */
	    currentpin = (GPIO_InitStruct.GPIO_Pin) & pos;

	    if (currentpin == pos)
	    {
	      GPIOx->MODER  &= ~(GPIO_MODER_MODER0 << (pinpos * 2));
	      GPIOx->MODER |= (((uint32_t)GPIO_InitStruct.GPIO_Mode) << (pinpos * 2));

	      if ((GPIO_InitStruct.GPIO_Mode == GPIO_Mode_OUT) || (GPIO_InitStruct.GPIO_Mode == GPIO_Mode_AF))
	      {
	        /* Check Speed mode parameters */
	        assert_param(IS_GPIO_SPEED(GPIO_InitStruct.GPIO_Speed));

	        /* Speed mode configuration */
	        GPIOx->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (pinpos * 2));
	        GPIOx->OSPEEDR |= ((uint32_t)(GPIO_InitStruct.GPIO_Speed) << (pinpos * 2));

	        /* Check Output mode parameters */
	        assert_param(IS_GPIO_OTYPE(GPIO_InitStruct.GPIO_OType));

	        /* Output mode configuration*/
	        GPIOx->OTYPER  &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)pinpos)) ;
	        GPIOx->OTYPER |= (uint16_t)(((uint16_t)GPIO_InitStruct.GPIO_OType) << ((uint16_t)pinpos));
	      }

	      /* Pull-up Pull down resistor configuration*/
	      GPIOx->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)pinpos * 2));
	      GPIOx->PUPDR |= (((uint32_t)GPIO_InitStruct.GPIO_PuPd) << (pinpos * 2));
	    }
	  }
	}
};



#endif /* MYHEADERS_BOARD_IO_HPP_ */
