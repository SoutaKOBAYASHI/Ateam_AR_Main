#include <cmath>

#include "r1370.hpp"

void R1370::receive_data_(const uint8_t receive_byte)
{
	static uint8_t recive_count = 0;
	static std::array<uint8_t, 15> recive_data;
	static uint8_t check_sum = 0;

	constexpr float receive_interval = 0.010f;

	recive_data.at(recive_count) = receive_byte;
	if(recive_data.at(0) != 0xAA)return;
	if(recive_count == 1 && recive_data.at(1) != 0x00)
	{
		recive_count = 0;
		return;
	}
	if(recive_count == 14)
	{
		for(uint8_t i = 2;i < 13;++i)check_sum = static_cast<uint8_t>(recive_data.at(i) + check_sum);
		if(recive_data.at(14) == check_sum)
		{
			/*deg/sec*/
			int32_t receive_angle_vel = ((static_cast<int32_t>(recive_data.at(5)) & 0x00FF) | ((static_cast<int32_t>(recive_data.at(6)) << 8) & 0xFF00));
			if(receive_angle_vel > INT16_MAX)receive_angle_vel |= 0xFFFF0000;

			angle_vel_ = M_PI * static_cast<float>(receive_angle_vel) / (DATA_GAIN_ * 180.0f);
			angle_ += receive_interval * angle_vel_;
		}
		recive_count = 0;
		check_sum = 0;
	}
	else
	{
		recive_count++;
	}
	return;
}
