/*
 * uart.hpp
 *
 *  Created on: 2019/03/29
 *      Author: User
 */

#ifndef MYHEADERS_UART_HPP_
#define MYHEADERS_UART_HPP_

#include <vector>
#include <array>
#include <string>
#include <functional>
#include <map>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

enum class uartName{ uart1, uart2 };

class UART
{
public:
	UART() = delete;
	UART(USART_TypeDef* const use_uart_name) : use_uart_name_(use_uart_name){}

	inline uartName useUartName () const { return use_uart_name_ == USART2 ? uartName::uart1 : uartName::uart2; }

	/*transmit a byte.*/
	inline void transmitData(const uint8_t sendByte)
	{
		while(!USART_GetFlagStatus(use_uart_name_, USART_FLAG_TXE));
		USART_SendData(use_uart_name_, static_cast<uint16_t>(sendByte));
	}

	/*overloaded functions of transmitting.*/
	inline void transmitData(const char sendByte) { transmitData(static_cast<uint8_t>(sendByte)); }

	inline void transmitData(const uint8_t* pSendData, const size_t size) { for (size_t i = 0; i < size; i++)transmitData(*(pSendData + i)); }
	inline void transmitData(const char* pSendData, const size_t size) { for (size_t i = 0; i < size; i++)transmitData(*(pSendData + i)); }

	inline void transmitData(const std::string & sendString) { transmitData(sendString.c_str(), sendString.size()); }
	template<size_t S>
	inline void transmitData(const std::array<uint8_t, S> & sendDataArray) { for (auto i : sendDataArray)transmitData(static_cast<uint8_t>(i)); }
	template<size_t S>
	inline void transmitData(const std::array<char, S> & sendDataArray) { for (auto i : sendDataArray)transmitData(static_cast<uint8_t>(i)); }
	inline void transmitData(const std::vector<uint8_t> & sendVector) { for (auto i : sendVector)transmitData(i); }

	inline uint8_t receiveData()
	{
		while(!USART_GetFlagStatus(use_uart_name_, USART_FLAG_RXNE));
		return static_cast<uint8_t>(USART_ReceiveData(use_uart_name_));
	}
private:
	USART_TypeDef* const use_uart_name_;
};

template<uartName useUartName, uint32_t setBaudrate = 9600, uint8_t IRQ_priority = 0>
class UART_Initialize
{
public:
	UART_Initialize() : uart_interface(useUartName_())
	{
		uartInit_();
		pinsInit_();
	}

	UART uart_interface;
private:
	void pinsInit_()
	{
		constexpr uint32_t RCC_AHB1_PERIPH =
				useUartName == uartName::uart1 ? RCC_AHB1Periph_GPIOA : RCC_AHB1Periph_GPIOB ;

		RCC_AHB1PeriphClockCmd(RCC_AHB1_PERIPH, ENABLE);

		constexpr uint16_t USE_UART_PINS =
				useUartName == uartName::uart1 ? (GPIO_Pin_2 | GPIO_Pin_3) : (GPIO_Pin_10 | GPIO_Pin_11) ;

		constexpr GPIOMode_TypeDef USE_PINS_MODE = GPIO_Mode_AF;
		constexpr GPIOSpeed_TypeDef USE_PINS_SPEED = GPIO_Speed_100MHz;
		constexpr GPIOOType_TypeDef USE_PINS_OTYPE = GPIO_OType_OD;
		constexpr GPIOPuPd_TypeDef USE_PINS_PUPD = GPIO_PuPd_UP;

		constexpr GPIO_InitTypeDef GPIO_INIT_STRUCT =
		{
				USE_UART_PINS,
				USE_PINS_MODE,
				USE_PINS_SPEED,
				USE_PINS_OTYPE,
				USE_PINS_PUPD
		};

		GPIO_TypeDef* const useGpioPort =
				useUartName == uartName::uart1 ? GPIOA : GPIOB;

		GPIO_Init_reference_(useGpioPort, GPIO_INIT_STRUCT);

		constexpr uint8_t GPIO_AF  =
				useUartName == uartName::uart1 ? GPIO_AF_USART2	: GPIO_AF_USART3;

		constexpr std::pair<uint16_t, uint16_t> GPIO_PIN_SOURCE =
				useUartName == uartName::uart1 ? std::make_pair(GPIO_PinSource2, GPIO_PinSource3) : std::make_pair(GPIO_PinSource10, GPIO_PinSource11);

		GPIO_PinAFConfig(useGpioPort, GPIO_PIN_SOURCE.first, GPIO_AF);
		GPIO_PinAFConfig(useGpioPort, GPIO_PIN_SOURCE.second, GPIO_AF);
	}
	void uartInit_()
	{
		constexpr uint32_t RCC_APB1PERIPH =
				useUartName == uartName::uart1 ? RCC_APB1Periph_USART2 : RCC_APB1Periph_USART3;

		RCC_APB1PeriphClockCmd(RCC_APB1PERIPH, ENABLE);

		USART_InitTypeDef USART_InitStruct;
		USART_StructInit(&USART_InitStruct);
		USART_InitStruct.USART_Mode		= USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStruct.USART_BaudRate	= setBaudrate;
		USART_Init(useUartName_(), &USART_InitStruct);

		USART_Cmd(useUartName_(), ENABLE);

		USART_ITConfig(useUartName_(), USART_IT_RXNE, ENABLE);

		NVIC_InitTypeDef NVIC_InitStruct;
		NVIC_InitStruct.NVIC_IRQChannel 					= 	useUartName == uartName::uart1 ? USART2_IRQn : USART3_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= IRQ_priority;
		NVIC_InitStruct.NVIC_IRQChannelCmd					= ENABLE;
		NVIC_Init(&NVIC_InitStruct);
	}
	constexpr USART_TypeDef* useUartName_()
	{
		return	useUartName == uartName::uart1 ? USART2 : USART3;
	}
	static void GPIO_Init_reference_(GPIO_TypeDef* const GPIOx, const GPIO_InitTypeDef& GPIO_InitStruct)
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

class UART_Interrupt
{
public:
	UART_Interrupt() = delete;

	UART_Interrupt(uartName use_uart_name, const std::function<void(uint8_t)>&& addFunc) : use_uart_name_(use_uart_name)
	{
		switch(use_uart_name_)
		{
		case uartName::uart1:
			uart1_call_functions_.insert(std::make_pair(this, addFunc));
			break;

		case uartName::uart2:
			uart2_call_functions_.insert(std::make_pair(this, addFunc));
			break;
		}
	}

	static void uart1Callback()
	{
		const uint8_t receive_byte = static_cast<uint8_t>(USART_ReceiveData(USART2));
		for(auto i : uart1_call_functions_)i.second(receive_byte);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}

	static void uart2Callback()
	{
		const uint8_t receive_byte = static_cast<uint8_t>(USART_ReceiveData(USART3));
		for(auto i : uart2_call_functions_)i.second(receive_byte);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}

	virtual ~UART_Interrupt()
	{
		switch(use_uart_name_)
		{
		case uartName::uart1:
			uart1_call_functions_.erase(this);
			break;

		case uartName::uart2:
			uart2_call_functions_.erase(this);
			break;
		}
	}
private:
	const uartName use_uart_name_;

	static std::map<UART_Interrupt* const,const std::function<void(uint8_t)>> uart1_call_functions_;
	static std::map<UART_Interrupt* const,const std::function<void(uint8_t)>> uart2_call_functions_;
};
#endif /* MYHEADERS_UART_HPP_ */
