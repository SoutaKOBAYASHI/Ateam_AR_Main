#ifndef GYRO_BASE_
#define GYRO_BASE_

#define _USE_MATH_DEFINES

#include <cmath>
#include <array>

#include "coordinate.hpp"

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

template<typename T>
class GyroBase
{
public:
	constexpr GyroBase() = default;
	virtual ~GyroBase()  = default;

	void reset();
	void readProcessing();

	inline Rectangular<T> getAccelRectangular() const		 { return accel_; }
	inline Rectangular<T> getRedGyroRectangular() const		 { return gyro_; }
	inline Rectangular<T> getMagRectangular() const			 { return mag_; }
	inline RollPitchYaw<T> getRadRollPitchYawAngle() const	 { return roll_pitch_yaw_; }
	inline Rectangular<T> getDegGyroRectangular() const		 { return Rectangular<T>{getDegXGyro(), getDegYGyro(), getDegZGyro()}; }
	inline RollPitchYaw<T> getDegRollPitchYawAngle() const	 { return RollPitchYaw<T>{getDegRoll(), getDegPitch(), getDegYaw()}; }

	inline void setAccelRectangular(const Rectangular<T> accel_rectangular)				{ accel_ = accel_rectangular; }
	inline void setRadGyroRectangular(const Rectangular<T> rad_gyro_rectangular)		{ gyro_ = rad_gyro_rectangular; }
	inline void setMagRectangular(const Rectangular<T> mag_rectangular)					{ mag_ = mag_rectangular; }
	inline void setRadRollPitchYawAngle(const RollPitchYaw<T> rad_roll_pitch_yaw_angle)	{ roll_pitch_yaw_ = rad_roll_pitch_yaw_angle; }
	inline void setDegGyroRectangular(const Rectangular<T> deg_gyro_rectangular)		{
		setDegXGyro(deg_gyro_rectangular.x);
		setDegYGyro(deg_gyro_rectangular.y);
		setDegZGyro(deg_gyro_rectangular.z);
	}
	inline void setDegRollPitchYawAngle(const RollPitchYaw<T> deg_roll_pitch_yaw_angle)	{
		setDegRoll(deg_roll_pitch_yaw_angle.roll); 
		setDegPitch(deg_roll_pitch_yaw_angle.pitch); 
		setDegYaw(deg_roll_pitch_yaw_angle.yaw);
	}

	constexpr T getXAccel() const	{ return accel_.x; }
	constexpr T getYAccel() const	{ return accel_.y; }
	constexpr T getZAccel() const	{ return accel_.z; }
	constexpr T getRadXGyro() const	{ return gyro_.x; }
	constexpr T getDegXGyro() const	{ return gyro_.x * static_cast<T>(DEGREE_OF_180 * RECIPROCAL_OF_PI); }
	constexpr T getRadYGyro() const	{ return gyro_.y; }
	constexpr T getDegYGyro() const	{ return gyro_.y * static_cast<T>(DEGREE_OF_180 * RECIPROCAL_OF_PI); }
	constexpr T getRadZGyro() const	{ return gyro_.z; }
	constexpr T getDegZGyro() const	{ return gyro_.z * static_cast<T>(DEGREE_OF_180 * RECIPROCAL_OF_PI); }
	constexpr T getXMag() const		{ return mag_.x; }
	constexpr T getYMag() const		{ return mag_.y; }
	constexpr T getZMag() const		{ return mag_.z; }
	constexpr T getRadRoll() const	{ return roll_pitch_yaw_.roll; }
	constexpr T getDegRoll() const	{ return roll_pitch_yaw_.roll * static_cast<T>(DEGREE_OF_180 * RECIPROCAL_OF_PI); }
	constexpr T getRadPitch() const	{ return roll_pitch_yaw_.pitch; }
	constexpr T getDegPitch() const	{ return roll_pitch_yaw_.pitch * static_cast<T>(DEGREE_OF_180 * RECIPROCAL_OF_PI); }
	constexpr T getRadYaw() const	{ return roll_pitch_yaw_.yaw; }
	constexpr T getDegYaw() const	{ return roll_pitch_yaw_.yaw * static_cast<T>(DEGREE_OF_180 * RECIPROCAL_OF_PI); }

	constexpr void setXAccel(const T x_accel)		{ accel_.x = x_accel; }
	constexpr void setYAccel(const T y_accel)		{ accel_.y = y_accel; }
	constexpr void setZAccel(const T z_accel)		{ accel_.z = z_accel; }
	constexpr void setRadXGyro(const T rad_x_gyro)	{ gyro_.x = rad_x_gyro; }
	constexpr void setDegXGyro(const T deg_x_gyro)	{ gyro_.x = deg_x_gyro * static_cast<T>(M_PI * RECIPROCAL_OF_180); }
	constexpr void setRadYGyro(const T rad_y_gyro)	{ gyro_.y = rad_y_gyro; }
	constexpr void setDegYGyro(const T deg_y_gyro)	{ gyro_.y = deg_y_gyro * static_cast<T>(M_PI * RECIPROCAL_OF_180); }
	constexpr void setRadZGyro(const T rad_z_gyro)	{ gyro_.z = rad_z_gyro; }
	constexpr void setDegZGyro(const T deg_z_gyro)	{ gyro_.z = deg_z_gyro * static_cast<T>(M_PI * RECIPROCAL_OF_180); }
	constexpr void setXMag(const T x_mag)			{ mag_.x = x_mag; }
	constexpr void setYMag(const T y_mag)			{ mag_.y = y_mag; }
	constexpr void setZMag(const T z_mag)			{ mag_.z = z_mag; }
	constexpr void setRadRoll(const T rad_roll)		{ roll_pitch_yaw_.roll = rad_roll; }
	constexpr void setDegRoll(const T deg_roll)		{ roll_pitch_yaw_.roll = deg_roll * static_cast<T>(M_PI * RECIPROCAL_OF_180); }
	constexpr void setRadPitch(const T rad_pitch)	{ roll_pitch_yaw_.pitch = rad_pitch; }
	constexpr void setDegPitch(const T deg_pitch)	{ roll_pitch_yaw_.pitch = deg_pitch * static_cast<T>(M_PI * RECIPROCAL_OF_180); }
	constexpr void setRadYaw(const T rad_yaw)		{ roll_pitch_yaw_.yaw = rad_yaw; }
	constexpr void setDegYaw(const T deg_yaw)		{ roll_pitch_yaw_.yaw = deg_yaw * static_cast<T>(M_PI * RECIPROCAL_OF_180); }

private:
	static constexpr double DEGREE_OF_180	  = 180.0;
	static constexpr double RECIPROCAL_OF_PI  = 1.0 / M_PI;
	static constexpr double RECIPROCAL_OF_180 = 1.0 / DEGREE_OF_180;

	Rectangular<T> accel_;
	Rectangular<T> gyro_;
	Rectangular<T> mag_;
	RollPitchYaw<T> roll_pitch_yaw_;
};

#endif