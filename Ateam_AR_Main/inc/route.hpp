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

#if (ZONE_IS_RED)
#include "red_zone/qualyfying/route_path_1.hpp"
#include "red_zone/qualyfying/route_path_2.hpp"
#include "red_zone/qualyfying/route_path_3.hpp"
#include "red_zone/qualyfying/route_path_4.hpp"
#include "red_zone/qualyfying/route_path_5.hpp"
#include "red_zone/qualyfying/route_path_6.hpp"
#include "red_zone/qualyfying/route_path_7.hpp"
#else
#include "blue_zone/qualyfying/route_path_1.hpp"
#include "blue_zone/qualyfying/route_path_2.hpp"
#include "blue_zone/qualyfying/route_path_3.hpp"
#include "blue_zone/qualyfying/route_path_4.hpp"
#include "blue_zone/qualyfying/route_path_5.hpp"
#include "blue_zone/qualyfying/route_path_6.hpp"
#include "blue_zone/qualyfying/route_path_7.hpp"
#endif

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

using Tpos = float;
using Tvel = uint32_t;

const std::array<std::vector<pathPoint<float>>, 7> running_path =
{
		route_path_1,
		route_path_2,
		route_path_3,
		route_path_4,
		route_path_5,
		route_path_6,
		route_path_7
};

#endif /* ROUTE_HPP_ */
