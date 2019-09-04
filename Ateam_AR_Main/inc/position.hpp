/*
 * position.hpp
 *
 *  Created on: 2019/09/01
 *      Author: User
 */

#ifndef POSITION_HPP_
#define POSITION_HPP_

#include <array>
#include <initializer_list>
#include <cmath>

template<typename _Tp>
class position
{
    static constexpr std::size_t elems_size_ = 3;
	std::array<_Tp, elems_size_> elems_;
public:


	constexpr position() : elems_({0, 0, 0}) {}
	constexpr position(std::initializer_list<_Tp> init)
	   : elems_({*init.begin(), *(init.begin() + 1), *(init.begin() + 2)}) { }

	constexpr position<_Tp> operator= (const position<_Tp>& pos_copied)
	{
		elems_ = {pos_copied.posX(), pos_copied.posY(), pos_copied.posTheta()};
		return *this;
	}

	constexpr position<_Tp> operator+= (const position<_Tp>& increment_pos)
	{
		elems_.at(0) += increment_pos.posX();
		elems_.at(1) += increment_pos.posY();
		elems_.at(2) += increment_pos.posTheta();
		return *this;
	}

	constexpr position<_Tp> operator-= (const position<_Tp>& decrement_pos)
	{
		elems_.at(0) += decrement_pos.posX();
		elems_.at(1) += decrement_pos.posY();
		elems_.at(2) += decrement_pos.posTheta();
		return *this;
	}

    constexpr _Tp posX() const { return elems_.at(0); }
	constexpr _Tp posX(const _Tp set_pos){ return elems_.at(0) = set_pos; }

	constexpr _Tp posY() const { return elems_.at(1); }
	constexpr _Tp posY(const _Tp set_pos){ return elems_.at(1) = set_pos; }

	constexpr _Tp posTheta() const { return elems_.at(2); }
	constexpr _Tp posTheta(const _Tp set_pos){ return elems_.at(2) = set_pos; }
};

template<typename _Tp>
constexpr _Tp caluc_distance(position<_Tp> pos1, position<_Tp> pos2)
{
	return std::sqrt(std::pow(pos1.posX() - pos2.posX(), 2) + std::pow(pos1.posY() - pos2.posY(), 2));
}

#endif /* POSITION_HPP_ */
