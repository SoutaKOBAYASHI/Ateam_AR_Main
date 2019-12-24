/*
 * auto_running.cpp
 *
 *  Created on: 2019/09/01
 *      Author: User
 */


#include "auto_running.hpp"

extern float run_vec_x, run_vec_y, run_vec_theta, run_vec_size; // for debug

int AutoRunning::pointControl(const uint8_t running_path_num)
{
	if(running_path_num >= running_path.size())
	{
		omni_wheel_.setRunningVector(0.0f, 0.0f, 0.0f);
		return -1;
	}

	static uint32_t point_num = 0;
	static uint8_t last_running_path_num = 0;
	if(last_running_path_num != running_path_num)point_num = 0;

	bool error_clear_ = caluc_distance(dead_reckoning_.pos(), running_path.at(running_path_num).at(point_num).get_pos()) <= auto_running_params_.acceptable_error;

	while(error_clear_)
	{
		if(point_num >= running_path.at(running_path_num).size() - 1)
		{
			point_num = running_path.at(running_path_num).size() - 1;
			break;
		}
		++point_num;
		error_clear_ = caluc_distance(dead_reckoning_.pos(), running_path.at(running_path_num).at(point_num).get_pos()) <= auto_running_params_.acceptable_error;
	}

	static PID<float, float> rolling_pid_(auto_running_params_.rolling_gain, dead_reckoning_.readPosTheta());
	//static PID<float, float> running_pid_(auto_running_params_.running_gain, 0);

	const float target_pos_x = running_path.at(running_path_num).at(point_num).get_pos().posX();
	const float target_pos_y = running_path.at(running_path_num).at(point_num).get_pos().posY();

	/*Calculation running vector, radius = deviation of position, theta = angle of running*/
	vector2d<float> run_vector =
		{ (target_pos_x - dead_reckoning_.readPosX()), (target_pos_y - dead_reckoning_.readPosY()) };

	/*PID step*/
	rolling_pid_.setPidEnableState(true);
	rolling_pid_.setLimitVal(auto_running_params_.rolling_speed);
	rolling_pid_.setTargetVal(running_path.at(running_path_num).at(point_num).get_pos().posTheta());
	rolling_pid_.update(dead_reckoning_.readPosTheta());

	const float run_vector_por_local = run_vector.getVector_pol().second - dead_reckoning_.pos().posTheta();

	run_vec_x		= run_vector.getVector_rec().first;
	run_vec_y		= run_vector.getVector_rec().second;
	run_vec_size	= run_vector.getVector_pol().first;
	run_vec_theta	= run_vector.getVector_pol().second;

	/*Drive wheel*/
	const float running_vel = running_path.at(running_path_num).at(point_num).velocity_abs() > 400.0f ? running_path.at(running_path_num).at(point_num).velocity_abs() : 400.0f;
	omni_wheel_.setRunningVector(running_vel, run_vector_por_local, static_cast<int32_t>(rolling_pid_.getControlVal()));

	last_running_path_num = running_path_num;

	if((point_num == running_path.at(running_path_num).size() - 1))
	{
		const bool error_is_accepteble_end = caluc_distance(dead_reckoning_.pos(), running_path.at(running_path_num).at(point_num).get_pos()) <= auto_running_params_.acceptable_error_endpoint;
		if(error_is_accepteble_end)return 1;
	}
	return 0;
}
