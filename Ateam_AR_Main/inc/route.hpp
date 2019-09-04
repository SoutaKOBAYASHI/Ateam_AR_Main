/*
 * route.hpp
 *
 *  Created on: 2019/09/01
 *      Author: User
 */

#ifndef ROUTE_HPP_
#define ROUTE_HPP_

#include "running_param_set.hpp"
#include <array>

using Tpos = float;
using Tvel = uint32_t;

constexpr std::array<std::array<RunningParamSet<Tpos, Tvel>, 17>, 2> ROUTE =
{{
	{
		std::make_pair<position<Tpos>, Tvel>({0.0f		, 0.0f		, 0.0f		} , 0		) ,
		std::make_pair<position<Tpos>, Tvel>({200.0f	, 200.0f	, 0.0f		} , 50		) ,
		std::make_pair<position<Tpos>, Tvel>({350.0f	, 400.0f	, 0.0f		} , 100		) ,
		std::make_pair<position<Tpos>, Tvel>({450.0f	, 600.0f	, 0.0f		} , 150		) ,
		std::make_pair<position<Tpos>, Tvel>({500.0f	, 800.0f	, 0.0f		} , 200		) ,
		std::make_pair<position<Tpos>, Tvel>({450.0f	, 1000.0f	, 0.0f		} , 250		) ,
		std::make_pair<position<Tpos>, Tvel>({350.0f	, 1200.0f	, 0.0f		} , 300		) ,
		std::make_pair<position<Tpos>, Tvel>({200.0f	, 1400.0f	, 0.0f		} , 350		) ,
		std::make_pair<position<Tpos>, Tvel>({0.0f		, 1600.0f	, 0.0f		} , 400		) ,
		std::make_pair<position<Tpos>, Tvel>({-200.0f	, 1800.0f	, 0.0f		} , 400		) ,
		std::make_pair<position<Tpos>, Tvel>({-350.0f	, 2000.0f	, 0.0f		} , 400		) ,
		std::make_pair<position<Tpos>, Tvel>({-450.0f	, 2200.0f	, 0.0f		} , 350		) ,
		std::make_pair<position<Tpos>, Tvel>({-500.0f	, 2400.0f	, 0.0f		} , 250		) ,
		std::make_pair<position<Tpos>, Tvel>({-450.0f	, 2600.0f	, 0.0f		} , 200		) ,
		std::make_pair<position<Tpos>, Tvel>({-350.0f	, 2800.0f	, 0.0f		} , 150		) ,
		std::make_pair<position<Tpos>, Tvel>({-200.0f	, 3000.0f	, 0.0f		} , 100		) ,
		std::make_pair<position<Tpos>, Tvel>({0.0f		, 3200.0f	, 0.0f		} , 50		)
	}
	,
	{
		std::make_pair<position<Tpos>, Tvel>({0.0f		, 3200.0f	, 0.0f		} , 0		) ,
		std::make_pair<position<Tpos>, Tvel>({200.0f	, 3000.0f	, 0.0f		} , 50		) ,
		std::make_pair<position<Tpos>, Tvel>({350.0f	, 2800.0f	, 0.0f		} , 100		) ,
		std::make_pair<position<Tpos>, Tvel>({450.0f	, 2600.0f	, 0.0f		} , 150		) ,
		std::make_pair<position<Tpos>, Tvel>({500.0f	, 2400.0f	, 0.0f		} , 200		) ,
		std::make_pair<position<Tpos>, Tvel>({450.0f	, 2200.0f	, 0.0f		} , 250		) ,
		std::make_pair<position<Tpos>, Tvel>({350.0f	, 2000.0f	, 0.0f		} , 300		) ,
		std::make_pair<position<Tpos>, Tvel>({200.0f	, 1800.0f	, 0.0f		} , 350		) ,
		std::make_pair<position<Tpos>, Tvel>({0.0f		, 1600.0f	, 0.0f		} , 400		) ,
		std::make_pair<position<Tpos>, Tvel>({-200.0f	, 1400.0f	, 0.0f		} , 400		) ,
		std::make_pair<position<Tpos>, Tvel>({-350.0f	, 1200.0f	, 0.0f		} , 400		) ,
		std::make_pair<position<Tpos>, Tvel>({-450.0f	, 1000.0f	, 0.0f		} , 350		) ,
		std::make_pair<position<Tpos>, Tvel>({-500.0f	, 800.0f	, 0.0f		} , 250		) ,
		std::make_pair<position<Tpos>, Tvel>({-450.0f	, 600.0f	, 0.0f		} , 200		) ,
		std::make_pair<position<Tpos>, Tvel>({-350.0f	, 400.0f	, 0.0f		} , 150		) ,
		std::make_pair<position<Tpos>, Tvel>({-200.0f	, 200.0f	, 0.0f		} , 100		) ,
		std::make_pair<position<Tpos>, Tvel>({0.0f		, 0.0f		, 0.0f		} , 50		)
	}
}};

#endif /* ROUTE_HPP_ */
