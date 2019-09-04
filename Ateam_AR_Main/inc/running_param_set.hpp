/*
 * running_paramset.hpp
 *
 *  Created on: 2019/09/01
 *      Author: User
 */

#ifndef RUNNING_PARAM_SET_HPP_
#define RUNNING_PARAM_SET_HPP_

#include <utility>
#include "position.hpp"

template<typename Tpos, typename Tvel>
class RunningParamSet
{
	std::pair<position<Tpos>, Tvel> running_param_set_;
public:
	constexpr RunningParamSet(){}
	constexpr RunningParamSet(const position<Tpos>& pos, const Tvel vel)
		: running_param_set_({pos, vel}){}
	constexpr RunningParamSet(const std::pair<position<Tpos>, Tvel>& set_param)
			: running_param_set_(set_param){}

	constexpr position<Tpos> readPos() const { return running_param_set_.first; }
	constexpr Tvel readVel() const { return running_param_set_.second; }
};

#endif /* RUNNING_PARAM_SET_HPP_ */
