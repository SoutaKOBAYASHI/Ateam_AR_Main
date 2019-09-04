/*
 * vector2d.hpp
 *
 *  Created on: 2019/09/01
 *      Author: User
 */

#include <initializer_list>
#include <utility>
#include <cmath>
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

template<typename _Tp>
class vector2d
{
private:
	_Tp point_x_;
	_Tp point_y_;

public:
	constexpr vector2d() : point_x_(0), point_y_(0) { }
	constexpr vector2d(std::initializer_list<_Tp> init)
		: point_x_(*init.begin()), point_y_(*(init.begin()+1)) { }
	constexpr vector2d(vector2d<_Tp>& copy)
		: point_x_(copy.getVector_rec().first), point_y_(copy.getVector_rec().second) { }

	constexpr std::pair<_Tp, _Tp> getVector_rec(){ return std::make_pair(point_x_, point_y_); }
	constexpr std::pair<_Tp, _Tp> getVector_pol()
	{
		_Tp rad = std::sqrt((point_x_* point_x_) + (point_y_* point_y_));
		_Tp theta = std::atan2(point_x_, point_y_);
		return std::make_pair(rad, theta);
	}
};

template<typename _Tp>
constexpr vector2d<_Tp> make_vector2d(_Tp point_x, _Tp point_y)
{
	vector2d<_Tp> made_vector = {point_x, point_y};
	return  made_vector;
}
