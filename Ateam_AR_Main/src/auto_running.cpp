/*
 * auto_running.cpp
 *
 *  Created on: 2019/09/01
 *      Author: User
 */


#include <auto_running.hpp>

extern float x, y, theta;

int AutoRunning::pointControl(const uint8_t route_num)
{
	if(route_num >= ROUTE.size())
	{
		omni_wheel_.setRunningVector(0.0f, 0.0f);
		return -1;
	}

	static uint32_t point_num = 0;
	static uint8_t last_route_num = 0;
	if(last_route_num != route_num)point_num = 0;

	bool error_clear_ = caluc_distance(dead_reckoning_.pos(), ROUTE.at(route_num).at(point_num).readPos()) <= ERROR_;

	if(error_clear_)
		point_num = (point_num >= ROUTE.at(route_num).size() - 1) ? (ROUTE.at(route_num).size() - 1) : (point_num + 1);

	error_clear_ = caluc_distance(dead_reckoning_.pos(), ROUTE.at(route_num).at(point_num).readPos()) <= ERROR_;

	static PID<float, float> rolling_pid_(ROLLING_GAIN_, dead_reckoning_.readPosTheta());
	static PID<float, float> running_pid_(RUNNING_GAIN_, 0);

	const float target_pos_x = ROUTE.at(route_num).at(point_num).readPos().posX();
	const float target_pos_y = ROUTE.at(route_num).at(point_num).readPos().posY();

	/*Calculation running vector, radius = deviation of position, theta = angle of running*/
	vector2d<float> run_vector =
		{ target_pos_x - dead_reckoning_.readPosX(), target_pos_y - dead_reckoning_.readPosY()};

	/*PID step*/
	running_pid_.setPidEnableState(true);
	running_pid_.setLimitVal(ROUTE.at(route_num).at(point_num).readVel());
	running_pid_.setTargetVal(run_vector.getVector_pol().first);
	running_pid_.update(0.0f);

	rolling_pid_.setPidEnableState(true);
	rolling_pid_.setLimitVal(ROLLING_SPEED_);
	rolling_pid_.setTargetVal(ROUTE.at(route_num).at(point_num).readPos().posTheta());
	rolling_pid_.update(dead_reckoning_.readPosTheta());

	/*Drive wheel*/
	omni_wheel_.setRunningVector
		((running_pid_.getControlVal()), run_vector.getVector_pol().second - (M_PI * 0.5) - dead_reckoning_.pos().posTheta(), static_cast<int32_t>(rolling_pid_.getControlVal()));

	last_route_num = route_num;

	x = dead_reckoning_.pos().posX();
	y = dead_reckoning_.pos().posY();
	theta = dead_reckoning_.pos().posTheta();

	if(error_clear_ && (point_num == ROUTE.at(route_num).size() - 1)) return 1;
	return 0;
}
