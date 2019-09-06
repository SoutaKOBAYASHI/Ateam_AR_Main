/*
 * sequence.cpp
 *
 *  Created on: 2019/09/02
 *      Author: User
 */

#include "sequence.hpp"

void Sequence::runSequence()
{
	auto_running_.deadReckoningUpdate();

	static uint32_t now_sequence_num_ = 0;
	switch(now_sequence_num_)
	{
	case 0:
		auto_running_.dead_reckoning_.setPos(-600.0f, 1800.0f, M_PI);
		received_msg_ = 0;
		if(!start_switch_.readNowState())
		{
			++now_sequence_num_;
		}
		break;

	case 1:
		sendCmd(0x01);
		++now_sequence_num_;
		break;

	case 2:
		if(auto_running_.pointControl(0))
		{
			++now_sequence_num_;
		}
		break;

	case 3:
		if(received_msg_ == 0x01)
		{
			sendCmd(0x02);
			++now_sequence_num_;
		}
		break;

	case 4:
		if(received_msg_ == 0x02)
		{
			sendCmd(0x03);
			++now_sequence_num_;
		}
		break;

	case 5:
		if(auto_running_.pointControl(1))
		{
			++now_sequence_num_;
		}
		break;

	case 6:
		if(received_msg_ == 0x03)
		{
			sendCmd(0x04);
			++now_sequence_num_;
		}
		break;

	case 7:
		if(received_msg_ == 0x04)
		{
			sendCmd(0x05);
			++now_sequence_num_;
		}
		break;

	case 8:
		if(auto_running_.pointControl(2) && received_msg_ == 0x05)
		{
			sendCmd(0x06);
			++now_sequence_num_;
		}
		break;

	case 9:
		if(received_msg_ == 0x06)
		{
			sendCmd(0x07);
			++now_sequence_num_;
		}
		break;

	case 10:
		if(received_msg_ == 0x07)
		{
			sendCmd(0x08);
			++now_sequence_num_;
		}
		break;

	case 11:
		if(auto_running_.pointControl(3))
		{
			++now_sequence_num_;
		}
		break;
	}
}

void Sequence::canReceiveMsg(const CanRxMsg& receive_msg)
{
	if(receive_msg.Data[0] == arm_ctrl_board_add_)
	{
		received_msg_ = receive_msg.Data[1];
	}
}
