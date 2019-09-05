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

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

using Tpos = float;
using Tvel = uint32_t;

constexpr std::array<std::array<RunningParamSet<Tpos, Tvel>, 12>, 3> ROUTE =
{{
	{
		std::make_pair<position<Tpos>, Tvel>({-600.0f	, 1800.0f	, M_PI		} , 0	) ,
		std::make_pair<position<Tpos>, Tvel>({-600.0f	, 2000.0f	, M_PI		} , 200	) ,
		std::make_pair<position<Tpos>, Tvel>({-600.0f	, 2200.0f	, M_PI		} , 400	) ,
		std::make_pair<position<Tpos>, Tvel>({-600.0f	, 2400.0f	, M_PI		} , 600	) ,
		std::make_pair<position<Tpos>, Tvel>({-600.0f	, 2600.0f	, M_PI		} , 600	) ,
		std::make_pair<position<Tpos>, Tvel>({-600.0f	, 3600.0f	, M_PI		} , 600	) ,
		std::make_pair<position<Tpos>, Tvel>({-700.0f	, 4500.0f	, M_PI		} , 400	) ,
		std::make_pair<position<Tpos>, Tvel>({-800.0f	, 5000.0f	, M_PI		} , 300	) ,
		std::make_pair<position<Tpos>, Tvel>({-1000.0f	, 5200.0f	, M_PI		} , 300	) ,
		std::make_pair<position<Tpos>, Tvel>({-1400.0f	, 5400.0f	, M_PI		} , 400	) ,
		std::make_pair<position<Tpos>, Tvel>({-2550.0f	, 5400.0f	, M_PI		} , 400	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 4749.0f	, M_PI		} , 200	) ,
	}
	,
	{
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 4749.0f	, M_PI		} , 100	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 4749.0f	, M_PI		} , 100	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 4749.0f	, M_PI		} , 100	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 4749.0f	, M_PI		} , 100	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 4749.0f	, M_PI		} , 100	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 4749.0f	, M_PI		} , 100	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 4749.0f	, M_PI		} , 100	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 4749.0f	, M_PI		} , 100	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 4749.0f	, M_PI		} , 100	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 5000.0f	, M_PI		} , 200	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 5800.0f	, M_PI		} , 400	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 6251.0f	, M_PI		} , 200	)
	}
	,
	{
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 6251.0f	, M_PI		} , 100	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 6251.0f	, M_PI		} , 100	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 6251.0f	, M_PI		} , 100	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 6251.0f	, M_PI		} , 100	) ,
		std::make_pair<position<Tpos>, Tvel>({-2750.0f	, 6000.0f	, M_PI		} , 200	) ,
		std::make_pair<position<Tpos>, Tvel>({-2350.0f	, 5600.0f	, M_PI		} , 400	) ,
		std::make_pair<position<Tpos>, Tvel>({-1000.0f	, 5600.0f	, M_PI		} , 400	) ,
		std::make_pair<position<Tpos>, Tvel>({-800.0f	, 5800.0f	, M_PI*0.5f	} , 400	) ,
		std::make_pair<position<Tpos>, Tvel>({-800.0f	, 7000.0f	, M_PI*0.5f	} , 400	) ,
		std::make_pair<position<Tpos>, Tvel>({-1200.0f	, 7600.0f	, M_PI*0.5f	} , 400	) ,
		std::make_pair<position<Tpos>, Tvel>({-2200.0f	, 7600.0f	, M_PI*0.5f	} , 300	) ,
		std::make_pair<position<Tpos>, Tvel>({-2200.0f	, 8084.0f	, M_PI*0.5f	} , 200	) ,
	}

}};

#endif /* ROUTE_HPP_ */
