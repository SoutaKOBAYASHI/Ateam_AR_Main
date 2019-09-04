/*
 * pid.hpp
 *
 *  Created on: 2019/09/02
 *      Author: User
 */

#ifndef PID_HPP_
#define PID_HPP_

#include <initializer_list>
#include <limits>

template<typename _Tp>
class pid_gain
{
public:
	const _Tp p;
	const _Tp i;
	const _Tp d;
	constexpr pid_gain() = delete;
	constexpr pid_gain(std::initializer_list<_Tp> init) :
		p(*init.begin()), i(*(init.begin() + 1)), d(*(init.begin() + 2)) { }
};

template<typename _Tp>
constexpr pid_gain<_Tp> make_pid_gain_set(_Tp _p, _Tp _i, _Tp _d)
{
	constexpr pid_gain<_Tp> gain_set = {_p, _i, _d};
	return  gain_set;
}

template<typename _Tio, typename _Tgain>
class PID
{
private:
	const pid_gain<_Tgain> gain_;

	_Tio target_val_ = 0;
	const _Tio& now_val_;

	_Tio control_val_limited_ = 0;
	_Tio control_val_ = 0;
	_Tio limit_val_ = std::numeric_limits<_Tio>::min();

	bool is_pid_enable_ = false;

	_Tgain  dev_last = 0, dev_before_last = 0;
public:
	PID() = delete;
	PID(const pid_gain<_Tgain>& pid_gains, const _Tio& now_val) :
		gain_(pid_gains), now_val_(now_val) {}

	inline _Tio setTargetVal(const _Tio set_val){ return target_val_ = set_val; }
	inline _Tio getTargetVal() const { return target_val_; }

	inline _Tio setControlVal(const _Tio set_val){ return control_val_limited_ = set_val; }
	inline _Tio getControlVal() const { return control_val_limited_; }

	inline  _Tio setPidEnableState(const bool new_state){ return is_pid_enable_ = new_state; }
	inline bool gisPidEnable() const { return is_pid_enable_; }

	inline _Tio setLimitVal(const _Tio set_val){ return limit_val_ = set_val; }
	inline _Tio getLimitVal() const { return limit_val_; }

	void update(const _Tio now_val)
	{
		/*pid type is speed*/
		const _Tio control_val_last = control_val_;

		const _Tgain dev_now = getTargetVal() - now_val;

		const _Tgain
			term_p = gain_.p * (dev_now - dev_last),
			term_i = gain_.i * dev_now,
			term_d = gain_.d * ((dev_now - dev_last) - (dev_last - dev_before_last));

		const _Tio add_terms = static_cast<_Tio>(term_p + term_i + term_d);

		const _Tio sum = add_terms + control_val_last;

		const _Tio new_set_val =
				std::abs(sum) >= std::abs(getLimitVal()) ?
					( sum > 0 ? getLimitVal() : -getLimitVal() ) : sum ;

		if(is_pid_enable_)
		{
			setControlVal(new_set_val);
		}

		control_val_ = sum;
		dev_before_last = dev_last;
		dev_last = dev_now;
	}
};

#endif /* PID_HPP_ */
