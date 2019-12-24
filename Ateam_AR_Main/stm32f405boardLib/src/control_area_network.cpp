/*
 * control_area_network.cpp
 *
 *  Created on: 2019/07/27
 *      Author: SotaKobayashi
 */

#include "control_area_network.hpp"

std::map<CAN_Interrupt* const,const std::function<void(const CanRxMsg&)>> CAN_Interrupt::can_call_functions_;

void ControlAreaNetwork::sendData(uint8_t *Data, uint8_t DataLenge, uint8_t Address)
{
	while(!(CAN1->TSR & CAN_TSR_TME0) || !(CAN1->TSR & CAN_TSR_TME1) || !(CAN1->TSR & CAN_TSR_TME2)); //To wait while mail boxes are pending.
	CanTxMsg CanTxMsgStructure;
	CanTxMsgStructure.StdId		= static_cast<uint32_t>(Address);
	CanTxMsgStructure.IDE		= CAN_ID_STD;
	CanTxMsgStructure.RTR		= CAN_RTR_DATA;
	CanTxMsgStructure.DLC		= DataLenge;
	for(uint8_t i = 0 ; i < DataLenge ; i++)CanTxMsgStructure.Data[i] = Data[i];
	CAN_Transmit(CAN1 , &CanTxMsgStructure);
}

void ControlAreaNetwork::sendRemote(uint8_t Address)
{
	while(!(CAN1->TSR & CAN_TSR_TME0) || !(CAN1->TSR & CAN_TSR_TME1) || !(CAN1->TSR & CAN_TSR_TME2)); //To wait while mail boxes are pending.
	CanTxMsg CanTxMsgStructure;
	CanTxMsgStructure.StdId		= static_cast<uint32_t>(Address);
	CanTxMsgStructure.IDE		= CAN_ID_STD;
	CanTxMsgStructure.RTR		= CAN_RTR_REMOTE;
	CanTxMsgStructure.DLC		= 0;
	CAN_Transmit(CAN1 , &CanTxMsgStructure);
	return;
}

extern "C"
{
	void CAN1_RX0_IRQHandler()
	{
		CAN_Interrupt::callback();
	}
}
