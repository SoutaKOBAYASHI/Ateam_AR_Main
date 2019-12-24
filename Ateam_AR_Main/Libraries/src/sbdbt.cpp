/*
 * sbdbt.cpp
 *
 *  Created on: 2019/10/09
 *      Author: SotaKobayashi
 */
#include "sbdbt.hpp"

void SbdbtDualShock::receiveByte(const uint8_t receive_byte)
{
	constexpr uint8_t START_BYTE = 0x80;

	uint16_t check_sum = 0;
	static decltype(received_data_arr_) input_data_arr;
	static uint8_t count;

	input_data_arr.at(count) = receive_byte;

	if(input_data_arr.at(0) != START_BYTE)
	{
		count = 0;
		return;
	}

	if(count == 7){
		now_timeout_count_ = 0.0f;
		check_sum = 0;
		for(uint8_t i = 1;i < received_data_arr_.size() - 1;++i)check_sum += input_data_arr.at(i);
		check_sum &=  0x7F;
		if(check_sum == input_data_arr.at(7))
		{
			received_data_arr_ = input_data_arr;
			led_.toggle();
		}
		check_sum = 0;
		count = 0;
	}
	else
	{
		count++;
	}
}

vector2d<float> SbdbtDualShock::readAnalogPad(const Pad read_pad) const
{
	constexpr uint8_t OFFSET = 0x40;

	float x_val = 0.0f;
	float y_val = 0.0f;

	if(read_pad == Pad::LEFT)
	{
		x_val = -1.0f * (static_cast<float>(static_cast<int8_t>(received_data_arr_.at(3) - OFFSET)) / static_cast<float>(OFFSET));
		y_val = (static_cast<float>(static_cast<int8_t>(received_data_arr_.at(4) - OFFSET)) / static_cast<float>(OFFSET));
	}
	else
	{
		x_val = -1.0f * (static_cast<float>(static_cast<int8_t>(received_data_arr_.at(5) - OFFSET)) / static_cast<float>(OFFSET));
		y_val = (static_cast<float>(static_cast<int8_t>(received_data_arr_.at(6) - OFFSET)) / static_cast<float>(OFFSET));
	}

	vector2d<float> pad_read_val = { x_val, y_val };
	return pad_read_val;
}
