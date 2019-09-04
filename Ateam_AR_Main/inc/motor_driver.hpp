#ifndef __LIBRARIES_MOTOR_DRIVER_H__
#define __LIBRARIES_MOTOR_DRIVER_H__

#include <array>
#include <type_traits>
#include <cstring>

#ifdef STM32F0
#include "stm32f0xx.h"
#ifdef USE_STDPERIPH_DRIVER
#include "stm32f0xx_conf.h"
#endif
#endif

#include "control_area_network.hpp"

#ifdef STM32F3
#include "stm32f30x.h"
#ifdef USE_STDPERIPH_DRIVER
#include "stm32f30x_conf.h"
#endif
#endif

#ifdef STM32F4
#include "stm32f4xx.h"
#ifdef USE_STDPERIPH_DRIVER
#include "stm32f4xx_conf.h"
#endif
#endif

class MotorDriver
{
public:
	struct SendParamsType
	{
		float PIDgain_P		= 0.0;
		float PIDgain_I		= 0.0;
		float PIDgain_D		= 0.0;
		uint32_t SpeedMAX	= UINT32_MAX;
		uint32_t PPR		= 0;
	};

	MotorDriver() = delete;
	MotorDriver(const uint8_t driver_address, const SendParamsType& set_driver_params, ControlAreaNetwork& can_interface) :
		usingMotorDriverAddress_(driver_address),
		can_interface_(can_interface)
	{
		this->sendParams_ = set_driver_params;
		params_send_flag_ = true;
	}
	MotorDriver(uint8_t driver_address, ControlAreaNetwork& can_interface) :
		usingMotorDriverAddress_(driver_address),
		can_interface_(can_interface){}

	enum MotorDriveMode : uint8_t
	{
		Stop		= 0x80,
		DutyControl	= 0x00,
		PID			= 0x01
	};
	inline void setMotorDriveMode(MotorDriveMode set_mode)
	{
		this->nowMode_ = set_mode;
	}
	void setSpeed(int32_t speed);
	inline void setSpeed(int32_t speed, MotorDriveMode drive_mode)
	{
		setMotorDriveMode(drive_mode);
		setSpeed(speed);
	}

	virtual void update()
	{
		paramsSendTable_();
	}

	enum ParamSendMode : uint8_t
	{
		PIDgain_P	= 0x02,
		PIDgain_I	= 0x03,
		PIDgain_D	= 0x04,
		SpeedMAX	= 0x05,
		PPR			= 0x06
	};

	template<ParamSendMode SendMode>
	int8_t paramsSend(const float send_param)
	{
		static_assert(SendMode == PIDgain_P || SendMode == PIDgain_I || SendMode == PIDgain_D);

		const uint32_t SendParam_Converted = bitsConvertion_<uint32_t>(send_param);
		std::array<uint8_t, motorDriverDataArraySize_> SendData = {};

		if constexpr(SendMode == PIDgain_P)	sendParams_.PIDgain_P	= send_param;
		if constexpr(SendMode == PIDgain_I)	sendParams_.PIDgain_I	= send_param;
		if constexpr(SendMode == PIDgain_D)	sendParams_.PIDgain_D	= send_param;

		//if(params_send_flag_)return -1;

		SendData[0] = SendMode;
		for(std::size_t i = 1;i < SendData.size();i++)
			SendData[i] = (uint8_t)(SendParam_Converted >> (8 * (SendData.size() - (i+1))));

		can_interface_.sendData(SendData, usingMotorDriverAddress_);

		return 0;
	}

	template<ParamSendMode SendMode>
	int8_t paramsSend(const uint32_t send_param)
	{
		static_assert(SendMode == SpeedMAX || SendMode == PPR);

		std::array<uint8_t, motorDriverDataArraySize_> SendData = {};

		if constexpr (SendMode == SpeedMAX)	sendParams_.SpeedMAX	= send_param;
		if constexpr (SendMode == PPR)		sendParams_.PPR			= send_param;

		//if(params_send_flag_)return -1;

		SendData[0] = SendMode;
		for(std::size_t i = 1;i < SendData.size();i++)
			SendData[i] = (uint8_t)(send_param >> (8 * (SendData.size() - (i+1))));

		can_interface_.sendData(SendData, usingMotorDriverAddress_);


		return 0;
	}
	void paramsSend(const SendParamsType send_driver_params)
	{
		sendParams_ = send_driver_params;
		params_send_flag_ = true;
	}

	virtual ~MotorDriver(){}

private:
	const uint8_t usingMotorDriverAddress_;
	ControlAreaNetwork& can_interface_;

	SendParamsType sendParams_;

	MotorDriveMode nowMode_ = MotorDriveMode::Stop;

	static constexpr size_t motorDriverDataArraySize_ = 5;

	uint16_t count = 0;

	template<class T, class U>
	T bitsConvertion_(U conv_data)
	{
		static_assert(std::alignment_of_v<T> == std::alignment_of_v<U>, "size of T is no matching for that U.");

		constexpr size_t dataSize = std::alignment_of_v<T>;

		T returnValue;
		memcpy(&returnValue, &conv_data, dataSize);

		return returnValue;
	}

	bool params_send_flag_ = false;
	void paramsSendTable_();
};

#endif
