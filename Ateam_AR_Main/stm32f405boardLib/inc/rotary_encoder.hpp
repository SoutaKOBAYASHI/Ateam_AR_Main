/*
 * rotary_encoder.h
 *
 *  Created on: 2019/03/01
 *      Author: User
 */

#ifndef MYHEADERS_ROTARY_ENCODER_HPP_
#define MYHEADERS_ROTARY_ENCODER_HPP_

#include <type_traits>
#include <utility>

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

enum class encoderName : uint8_t
{
	RotEnc1,
	RotEnc2,
	RotEnc3,
	RotEnc4
};

template<encoderName useRotEnc, uint16_t setPeriod = UINT16_MAX>
class RotaryEncoder
{
private:

public:
	TIM_TypeDef* const useTimer;
	RotaryEncoder() : useTimer(getUseTimer_())
	{
		timerClockInit_();
		configEncoderInterfaceMode_();
		configEncoderPins_();
	}

	template<typename T>
	auto readCount() const -> std::enable_if_t<std::is_floating_point<T>::value, T>
		{ return static_cast<double>(static_cast<int16_t>(useTimer->CNT)); }

	template<typename T>
	auto readCount() const -> typename std::enable_if_t<std::is_unsigned<T>::value, T>
	{
		return static_cast<T>(useTimer->CNT);
	}

	template<typename T>
	auto readCount() -> typename std::enable_if<std::is_signed<T>::value && !std::is_floating_point<T>::value, T>::type
	{
		const uint32_t countValue = (uint32_t)useTimer->CNT;
		std::make_unsigned_t<T> returnValue = 0;
	    if(countValue > ((uint32_t)(useTimer->ARR/2)))
		{
	    	/*Complement representation*/
			returnValue = returnValue - 1;	//returnValue is 0x~FFFF.
			returnValue = returnValue - (std::make_unsigned_t<T>)useTimer->ARR;
			returnValue = returnValue + (std::make_unsigned_t<T>)countValue;
		}
		else
		{
			returnValue = (std::make_unsigned_t<T>)countValue;
		}
		return (T)returnValue;
	}

	inline void clearCount(){ useTimer->CNT = 0; }
	inline void setCount(uint32_t setValue){ useTimer->CNT = setValue; }

	virtual ~RotaryEncoder(){}

protected:
	inline void timerClockInit_()
	{
		constexpr uint32_t RCC_Periph =
				useRotEnc == encoderName::RotEnc1 ? RCC_APB2Periph_TIM1 :
				useRotEnc == encoderName::RotEnc2 ? RCC_APB1Periph_TIM3 :
				useRotEnc == encoderName::RotEnc3 ? RCC_APB1Periph_TIM4 : RCC_APB1Periph_TIM5;

		if constexpr (useRotEnc == encoderName::RotEnc1)
		{
			RCC_APB2PeriphClockCmd(RCC_Periph, ENABLE);
		}
		else
		{
			RCC_APB1PeriphClockCmd(RCC_Periph, ENABLE);
		}
	}

	inline void configEncoderInterfaceMode_()
	{
		TIM_EncoderInterfaceConfig(useTimer, TIM_EncoderMode_TI12 , TIM_ICPolarity_Rising , TIM_ICPolarity_Rising);
		TIM_Cmd(useTimer, ENABLE);

		TIM_SetAutoreload(useTimer, setPeriod);

		TIM_SetCounter(useTimer, 0);
	}

	void configEncoderPins_()
	{
		GPIO_TypeDef* const useGpioPort =
						useRotEnc == encoderName::RotEnc1 ? GPIOA :
						useRotEnc == encoderName::RotEnc2 ? GPIOA :
						useRotEnc == encoderName::RotEnc3 ? GPIOB : GPIOA;

		const uint32_t RCC_Periph =
						useGpioPort == GPIOA ? RCC_AHB1Periph_GPIOA : RCC_AHB1Periph_GPIOB;

		RCC_AHB1PeriphClockCmd(RCC_Periph, ENABLE);

		constexpr uint16_t useEncoderPins =
				useRotEnc == encoderName::RotEnc1 ? (GPIO_Pin_8 | GPIO_Pin_9) :
				useRotEnc == encoderName::RotEnc2 ? (GPIO_Pin_6 | GPIO_Pin_7) :
				useRotEnc == encoderName::RotEnc3 ? (GPIO_Pin_6 | GPIO_Pin_7) : (GPIO_Pin_0 | GPIO_Pin_1);

		constexpr GPIOMode_TypeDef usePinsMode = GPIO_Mode_AF;
		constexpr GPIOSpeed_TypeDef usePinsSpeed = GPIO_Speed_100MHz;
		constexpr GPIOOType_TypeDef usePinsOType = GPIO_OType_OD;
		constexpr GPIOPuPd_TypeDef usePinsPuPd = GPIO_PuPd_UP;

		constexpr GPIO_InitTypeDef gpioinitStruct =
		{
				useEncoderPins,
				usePinsMode,
				usePinsSpeed,
				usePinsOType,
				usePinsPuPd
		};

		constexpr uint8_t GPIO_AF  =
				useRotEnc == encoderName::RotEnc1 ? GPIO_AF_TIM1 :
				useRotEnc == encoderName::RotEnc2 ? GPIO_AF_TIM3 :
				useRotEnc == encoderName::RotEnc3 ? GPIO_AF_TIM4 : GPIO_AF_TIM5;

		constexpr std::pair<uint16_t, uint16_t> GPIO_PIN_SOURCE =
				useRotEnc == encoderName::RotEnc1 ? std::make_pair(GPIO_PinSource8, GPIO_PinSource9) :
				useRotEnc == encoderName::RotEnc2 ? std::make_pair(GPIO_PinSource6, GPIO_PinSource7) :
				useRotEnc == encoderName::RotEnc3 ? std::make_pair(GPIO_PinSource6, GPIO_PinSource7) : std::make_pair(GPIO_PinSource0, GPIO_PinSource1);

		GPIO_PinAFConfig(useGpioPort, GPIO_PIN_SOURCE.first, GPIO_AF);
		GPIO_PinAFConfig(useGpioPort, GPIO_PIN_SOURCE.second, GPIO_AF);
		GPIO_Init_reference(useGpioPort, gpioinitStruct);
	}
	constexpr TIM_TypeDef* getUseTimer_()
	{
		return	useRotEnc == encoderName::RotEnc1 ? TIM1 :
				useRotEnc == encoderName::RotEnc2 ? TIM3 :
				useRotEnc == encoderName::RotEnc3 ? TIM4 : TIM5;
	}
	static void GPIO_Init_reference(GPIO_TypeDef* const GPIOx, const GPIO_InitTypeDef& GPIO_InitStruct)
	{
	  uint32_t pinpos = 0x00, pos = 0x00 , currentpin = 0x00;

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
			/* Speed mode configuration */
			GPIOx->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (pinpos * 2));
			GPIOx->OSPEEDR |= ((uint32_t)(GPIO_InitStruct.GPIO_Speed) << (pinpos * 2));

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



#endif /* MYHEADERS_ROTARY_ENCODER_HPP_ */
