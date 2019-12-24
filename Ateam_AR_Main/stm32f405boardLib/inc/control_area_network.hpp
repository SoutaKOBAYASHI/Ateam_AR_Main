
#ifndef CONTROL_AREA_NETWORK_HPP__
#define CONTROL_AREA_NETWORK_HPP__

#include <array>
#include <map>
#include <queue>
#include <utility>
#include <functional>

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

enum class CAN_TimeQuanta : uint8_t
{
	tq1		= 0x00,
	tq2		= 0x01,
	tq3		= 0x02,
	tq4		= 0x03,
	tq5		= 0x04,
	tq6		= 0x05,
	tq7		= 0x06,
	tq8		= 0x07,
	tq9		= 0x08,
	tq10	= 0x09,
	tq11	= 0x0A,
	tq12	= 0x0B,
	tq13	= 0x0C,
	tq14	= 0x0D,
	tq15	= 0x0E,
	tq16	= 0x0F
};

class ControlAreaNetwork
{
public:
	template<size_t S>
	void sendData(const std::array<uint8_t, S> &SendDataArray, uint8_t Address)
	{
		static_assert( !(S > 8), "Size of SendDataArray has to be less than eight.");

		while(!(CAN1->TSR & CAN_TSR_TME0) || !(CAN1->TSR & CAN_TSR_TME1) || !(CAN1->TSR & CAN_TSR_TME2)); //To wait while mail boxes are pending.
		CanTxMsg CanTxMsgStructure;
		CanTxMsgStructure.StdId				= static_cast<uint32_t>(Address);
		CanTxMsgStructure.IDE				= CAN_ID_STD;
		CanTxMsgStructure.RTR				= CAN_RTR_DATA;
		CanTxMsgStructure.DLC	= S;
		for(uint8_t i = 0 ; i < CanTxMsgStructure.DLC ; i++)CanTxMsgStructure.Data[i] = SendDataArray[i];
		CAN_Transmit(CAN1 , &CanTxMsgStructure);
	}

	void sendData(uint8_t *Data, uint8_t DataLenge, uint8_t Address);

	void sendRemote(uint8_t Address);
};

template<uint8_t setAddress, CAN_TimeQuanta BS1_timeQuanta = CAN_TimeQuanta::tq4, CAN_TimeQuanta BS2_timeQuanta = CAN_TimeQuanta::tq2, uint8_t prescaler = 6>
class CAN_Initialize
{
public:
	CAN_Initialize()
	{
		static_assert(static_cast<uint8_t>(BS2_timeQuanta) <= static_cast<uint8_t>(CAN_TimeQuanta::tq8), "BS2_timeQuanta has to be less than 8.");

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);

		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_11 | GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
		GPIO_Init(GPIOA , &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOA , GPIO_PinSource11 , GPIO_AF_CAN1);
		GPIO_PinAFConfig(GPIOA , GPIO_PinSource12 , GPIO_AF_CAN1);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 , ENABLE);

		CAN_InitTypeDef CAN_InitStructure;
		CAN_StructInit(&CAN_InitStructure);
		CAN_InitStructure.CAN_Mode 		= CAN_Mode_Normal;
		CAN_InitStructure.CAN_TXFP 		= ENABLE;
		CAN_InitStructure.CAN_Prescaler = prescaler;
		CAN_InitStructure.CAN_BS1 = static_cast<uint8_t>(BS1_timeQuanta);
		CAN_InitStructure.CAN_BS2 = static_cast<uint8_t>(BS2_timeQuanta);
		CAN_Init(CAN1 , &CAN_InitStructure);

		CAN_FilterInitTypeDef CAN_FilterInitStructure;
		CAN_FilterInitStructure.CAN_FilterNumber			= 0;

		/*ID-Mask Mode*/

		CAN_FilterInitStructure.CAN_FilterMode 				= CAN_FilterMode_IdMask;
		CAN_FilterInitStructure.CAN_FilterScale				= CAN_FilterScale_32bit;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh		= 0xFFE0;
		CAN_FilterInitStructure.CAN_FilterMaskIdLow			= 0x0000;
		CAN_FilterInitStructure.CAN_FilterIdHigh			= static_cast<uint16_t>(static_cast<uint16_t>(setAddress) << 5);
		CAN_FilterInitStructure.CAN_FilterIdLow				= 0x0000;

		/*END*/

		CAN_FilterInitStructure.CAN_FilterFIFOAssignment	= CAN_FilterFIFO0;
		CAN_FilterInitStructure.CAN_FilterActivation		= ENABLE;
		CAN_FilterInit(&CAN_FilterInitStructure);

		CAN_ITConfig(CAN1 , CAN_IT_FMP0 , ENABLE);

		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel						= CAN1_RX0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd					= ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}

	ControlAreaNetwork can_interface;
}; // namespase CAN_Initialize

class CAN_Interrupt
{
public:
	CAN_Interrupt() = delete;

	CAN_Interrupt(const std::function<void(const CanRxMsg&)>&& addFunc)
	{
		can_call_functions_.insert(std::make_pair(this, addFunc));
	}

	static void callback()
	{
		CanRxMsg can_rx_msg_struct;
		CAN_Receive(CAN1, CAN_FIFO0, &can_rx_msg_struct);
		for(auto i : can_call_functions_)i.second(can_rx_msg_struct);
	}


	virtual ~CAN_Interrupt()
	{
		can_call_functions_.erase(this);
	}
private:
	static std::map<CAN_Interrupt* const,const std::function<void(const CanRxMsg&)>> can_call_functions_;
};
#endif
