/*
 * route.hpp
 *
 *  Created on: 2019/09/01
 *      Author: User
 */

#ifndef ROUTE_HPP_
#define ROUTE_HPP_

#include "PathPoint.hpp"
#include <array>
#include <vector>

#include "route_path_1.hpp"
#include "route_path_2.hpp"

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

using Tpos = float;
using Tvel = uint32_t;

const std::array<std::vector<pathPoint<float>>, 2> running_path =
{
		route_path_1,
		route_path_2
};

#endif /* ROUTE_HPP_ */
