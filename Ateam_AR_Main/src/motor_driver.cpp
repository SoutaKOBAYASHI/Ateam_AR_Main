#include "motor_driver.hpp"

void MotorDriver::setSpeed(int32_t speed)
{
	std::array<uint8_t, motorDriverDataArraySize_> SendData = {};
	SendData[0] = this->nowMode_;
	for(std::size_t i = 1 ; i < motorDriverDataArraySize_ ; i++)
		SendData[i] = (uint8_t)(speed >> 8 * (motorDriverDataArraySize_ - (i+1)));

	can_interface_.sendData(SendData, this->usingMotorDriverAddress_);
}

void MotorDriver::paramsSendTable_()
{
	constexpr uint16_t SendInterval_ms_ = 100;

	if(!params_send_flag_)return;

	switch(count)
	{
	case (ParamSendMode::PIDgain_P-ParamSendMode::PIDgain_P)*SendInterval_ms_:
		paramsSend<ParamSendMode::PIDgain_P>(sendParams_.PIDgain_P);
		break;

	case (ParamSendMode::PIDgain_I-ParamSendMode::PIDgain_P)*SendInterval_ms_:
		paramsSend<ParamSendMode::PIDgain_I>(sendParams_.PIDgain_I);
		break;

	case (ParamSendMode::PIDgain_D-ParamSendMode::PIDgain_P)*SendInterval_ms_:
		paramsSend<ParamSendMode::PIDgain_D>(sendParams_.PIDgain_D);
		break;

	case (ParamSendMode::SpeedMAX-ParamSendMode::PIDgain_P)*SendInterval_ms_:
		paramsSend<ParamSendMode::SpeedMAX>(sendParams_.SpeedMAX);
		break;

	case (ParamSendMode::PPR-ParamSendMode::PIDgain_P)*SendInterval_ms_:
		paramsSend<ParamSendMode::PPR>(sendParams_.PPR);
		params_send_flag_ = false;
		count = 0;
		break;

	default:
		break;
	}
	count++;
}
