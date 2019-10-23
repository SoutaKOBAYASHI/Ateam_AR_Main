/*
 * sequence.cpp
 *
 *  Created on: 2019/09/02
 *      Author: User
 */

#include "sequence.hpp"

extern float x, y, theta;

void Sequence::runSequence_()
{
	static uint32_t now_sequence_num_ = 0;

	constexpr float H1500_MECHA_FALL_POS_Y = 5700;
	constexpr float H2000_MECHA_FALL_POS_Y = 7700;

	bool h1500_mecha_falled = false;
	bool h2000_mecha_falled = false;

	auto_running_.deadReckoningUpdate();

	x = auto_running_.dead_reckoning_.readPosX();
	y = auto_running_.dead_reckoning_.readPosY();
	theta = auto_running_.dead_reckoning_.readPosTheta();

	static bool emergency_remote;
	if(received_msg_ == 0xFF)
	{
		emergency_remote = true;
	}
	else if(received_msg_ == 0xAA)
	{
		emergency_remote = false;
	}

	if(emergency_remote)
	{
		if(emergency_state_)set_tepe_led_color_(255, 0, 0);
		return;
	}

	if(now_sequence_num_ == 0)
	{
		set_tepe_led_color_(255, 255, 255);
	}
	else
	{
		set_tepe_led_color_(0, 255, 0);
	}

	if(emergency_state_)
	{
		emegency_state_indicator_.setNewState(LED_State::ON);
		now_sequence_num_ = 0;
	}
	else
	{
		emegency_state_indicator_.setNewState(LED_State::OFF);
	}

	switch(now_sequence_num_)
	{
	case 0:
		received_msg_ = 0;
		h1500_mecha_falled = false;
		h2000_mecha_falled = false;
		auto_running_.dead_reckoning_.setPos((ZONE_IS_RED ? 600 : -600), 1800, (ZONE_IS_RED ? M_PI : 0.0f));
		if(!start_switch_.readNowState())
		{
			sendCmd_(0x03);
			++now_sequence_num_;
		}
		break;

	case 1:
		if(auto_running_.pointControl(0))
		{
			sendCmd_(0x04);
			++now_sequence_num_;
		}
		break;

	case 2:
		//++now_sequence_num_;
		if(received_msg_ == 0x04)
		{
			++now_sequence_num_;
		}
		break;

	case 3:
		if(auto_running_.dead_reckoning_.readPosY() < H1500_MECHA_FALL_POS_Y && !h1500_mecha_falled)
		{
			sendCmd_(0x01);
			h1500_mecha_falled = true;
		}
		if(auto_running_.pointControl(1) && received_msg_ == 0x01)
		{
			sendCmd_(0x02);
			++now_sequence_num_;
		}
		break;

	case 4:
		//++now_sequence_num_;
		if(received_msg_ == 0x02)
		{
			++now_sequence_num_;
		}
		break;

	case 5:
		if(auto_running_.pointControl(2))
		{
			sendCmd_(0x05);
			++now_sequence_num_;
		}
		break;

	case 6:
		//++now_sequence_num_;
		if(received_msg_ == 0x05)
		{
			++now_sequence_num_;
		}
		break;

	case 7:
		if(auto_running_.pointControl(3))
		{
			sendCmd_(0x06);
			++now_sequence_num_;
		}
		break;

	case 8:
		//++now_sequence_num_;
		if(received_msg_ == 0x06)
		{
			sendCmd_(0x07);
			++now_sequence_num_;
		}
		break;

	case 9:
		//++now_sequence_num_;
		if(received_msg_ == 0x07)
		{
			++now_sequence_num_;
		}
		break;

	case 10:
		if(auto_running_.pointControl(4) && received_msg_ == 0x07)
		{
			sendCmd_(0x08);
			++now_sequence_num_;
		}
		break;

	case 11:
		//++now_sequence_num_;
		++now_sequence_num_;

		break;

	case 12:
		if(auto_running_.dead_reckoning_.readPosY() < H2000_MECHA_FALL_POS_Y && !h2000_mecha_falled)
		{
			sendCmd_(0x09);
			h1500_mecha_falled = true;
		}
		if(auto_running_.pointControl(5))
		{
			sendCmd_(0x0A);
			++now_sequence_num_;
		}
		break;

	case 13:
		//++now_sequence_num_;
		if(received_msg_ == 0x0A)
		{
			sendCmd_(0x00);
			++now_sequence_num_;
		}
		break;

	case 14:
		if(auto_running_.pointControl(6))
		{
			++now_sequence_num_;
		}
		break;

	}

	transmitPosData_();
}

void Sequence::canReceiveMsg_(const CanRxMsg& receive_msg)
{
	if(receive_msg.Data[0] == arm_ctrl_board_add_)
	{
		received_msg_ = receive_msg.Data[1];
	}
}

void Sequence::transmitPosData_()
{
	const uint8_t START_BYTE = 0x80;
	constexpr uint8_t TRANSMIT_DATA_SIZE = 11;
	std::array<uint8_t, TRANSMIT_DATA_SIZE> transmit_data = {};

	uint32_t theta_transmit_type = 0;
	uint8_t check_sum = 0;
	const float theta_float = auto_running_.pos().posTheta();
	std::memcpy(&theta_transmit_type, &theta_float, 4);

	transmit_data.at(0) = START_BYTE;
	transmit_data.at(1) = static_cast<uint8_t>(static_cast<int16_t>(auto_running_.pos().posX()) >> 8);
	transmit_data.at(2) = static_cast<uint8_t>(static_cast<int16_t>(auto_running_.pos().posX()));
	transmit_data.at(3) = static_cast<uint8_t>(static_cast<int16_t>(auto_running_.pos().posY()) >> 8);
	transmit_data.at(4) = static_cast<uint8_t>(static_cast<int16_t>(auto_running_.pos().posY()));
	transmit_data.at(5) = static_cast<uint8_t>(theta_transmit_type >> 24);
	transmit_data.at(6) = static_cast<uint8_t>(theta_transmit_type >> 16);
	transmit_data.at(7) = static_cast<uint8_t>(theta_transmit_type >> 8);
	transmit_data.at(8) = static_cast<uint8_t>(theta_transmit_type);
	transmit_data.at(9) = ZONE_IS_RED ? 0x00 : 0xFF;

	for(uint8_t i = 1;i < TRANSMIT_DATA_SIZE - 1;++i) check_sum += transmit_data.at(i);
	transmit_data.at(TRANSMIT_DATA_SIZE - 1) = check_sum;

	pc_uart_port_.transmitData(transmit_data);
}

void Sequence::receiveLrfData(const uint8_t receive_byte)
{

	constexpr uint8_t START_BYTE = 0x80;
	static uint8_t receive_count = 0;
	static std::array<uint8_t, 6> receive_data_arr = {};

	constexpr float LRF_WEIGHT = 0.45f;

	uint8_t check_sum = 0;

	int16_t receive_pos_x = 0;
	int16_t receive_pos_y = 0;

	receive_data_arr.at(receive_count) = receive_byte;
	if(receive_data_arr.at(0) == START_BYTE)
	{
		//set_tepe_led_color_(0,0,255);
		receive_count++;
		if(receive_count == 6)
		{
			//set_tepe_led_color_(255,0,0);
			for(uint8_t i = 1;i < 5;++i) check_sum += receive_data_arr.at(i);
			if(check_sum == receive_data_arr.at(5))
			{
				//set_tepe_led_color_(0,255,0);
				receive_pos_x = static_cast<int16_t>(receive_data_arr.at(1)) << 8 | static_cast<int16_t>(receive_data_arr.at(2));
				receive_pos_y = static_cast<int16_t>(receive_data_arr.at(3)) << 8 | static_cast<int16_t>(receive_data_arr.at(4));

				if(receive_pos_x != 0)
				{
					auto_running_.dead_reckoning_.setPosX((static_cast<float>(receive_pos_x) * LRF_WEIGHT) + (auto_running_.dead_reckoning_.readPosX() * (1.0f - LRF_WEIGHT)));
				}
				if(receive_pos_y != 0)
				{
					auto_running_.dead_reckoning_.setPosY((static_cast<float>(receive_pos_y) * LRF_WEIGHT) + (auto_running_.dead_reckoning_.readPosY() * (1.0f - LRF_WEIGHT)));
				}
			}
			receive_count = 0;
		}
	}
	else
	{
		receive_count = 0;
	}
}
