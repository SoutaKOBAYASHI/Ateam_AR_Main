#ifndef COORDINATE_HPP_
#define COOEDINATE_HPP_

template<typename T>
struct Rectangular
{
	T x;
	T y;
	T z;
};

template<typename T>
struct RollPitchYaw
{
	T roll;
	T pitch;
	T yaw;
};

#endif