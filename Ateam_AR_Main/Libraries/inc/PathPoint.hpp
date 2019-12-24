#pragma once
#include <type_traits>
#include <cmath>
#include "position.hpp"
#if __has_include("Eigen/core")
	#include "Eigen/core"
#endif


template<typename T>
class pathPoint{
	static_assert(std::is_floating_point_v<T>, "T must be a float type.");

private:
	position<T> _pos;
	T _velocity_abs;
	T _angular_velocity;

public:
	constexpr pathPoint(T x = 0.0, T y = 0.0, T angle = 0.0, T velocity_abs = 0.0, T angular_velocity = 0.0):
	_pos(x,y,angle), _velocity_abs(velocity_abs), _angular_velocity(angular_velocity){
	}

	//メンバ関数

	/*
	 * 原点からの距離算出関数
	 * 戻り値:距離
	 */
	inline constexpr auto point_norm() const -> decltype(_pos.posX() + 0.0f) { return sqrt(pow(_pos.posX(), 2.0) + pow(_pos.posY(), 2.0)); }//自動昇格でサイズ抑え

	/*
	 * 位置ベクトルの傾き算出関数
	 * 戻り値:角度
	 */
	constexpr auto point_angle() const ->decltype(_pos.posX() + 0.0f) { return atan2(_pos.posY(), _pos.posX()); }

	/*
	位置の取得
	*/
	constexpr inline position<T> get_pos() const { return _pos; }


	//アクセッサ
	constexpr inline T& x() &{ return _pos.posX(); }
	constexpr inline T& y() &{ return _pos.posY(); }
	constexpr inline T& angle() &{ return _pos.posTheta(); }
	constexpr inline T& velocity_abs() &{ return _velocity_abs; }
	constexpr inline T& angular_velocity() &{ return _angular_velocity; }
	constexpr inline const T& x() const & { return _pos.posX(); }
	constexpr inline const T& y() const & { return _pos.posY(); }
	constexpr inline const T& angle() const & { return _pos.posTheta(); }
	constexpr inline const T& velocity_abs() const & { return _velocity_abs; }
	constexpr inline const T& angular_velocity() const & { return _angular_velocity; }
	constexpr inline T x() const && { return _pos.posX(); }
	constexpr inline T y() const && { return _pos.posY(); }
	constexpr inline T angle() const && { return _pos.posTheta(); }
	constexpr inline T velocity_abs() const && { return _velocity_abs; }
	constexpr inline T angular_velocity() const &&{ return _angular_velocity; }

	//添え字オーバーロード
	constexpr const T& operator[](size_t index) const & { return *((&_pos.posX()) + index); }
	constexpr T& operator[](size_t index) & { return *((&_pos.posX()) + index); }
	constexpr T operator[](size_t index) const && { return *((&_pos.posX()) + index); }
};

/*floatに対する特殊化*/
template<>
constexpr auto pathPoint<float>::point_norm() const ->decltype(_pos.posX() + 0.0f) { return sqrtf(powf(_pos.posX(), 2.0f) + powf(_pos.posY(), 2.0f)); }
template<>
constexpr auto pathPoint<float>::point_angle() const ->decltype(_pos.posX() + 0.0f) { return atan2f(_pos.posY(), _pos.posX()); }

//算術オーバーロード
template<class Vector, typename T>
constexpr inline Vector operator+(const pathPoint<T>& l_operand, const pathPoint<T>& r_operand) {
	return Vector(l_operand.x() + r_operand.x(), l_operand.y() + r_operand.x());
}
template<class Vector, typename T>
constexpr inline Vector operator-(const pathPoint<T>& l_operand, const pathPoint<T>& r_operand) {
	return Vector(l_operand.x() - r_operand.x(), l_operand.y() - r_operand.x());
}
template<class Vector, typename T>
constexpr inline Vector operator*(const pathPoint<T>& l_operand, const T& r_operand) {
	return Vector(l_operand.x() * r_operand, l_operand.y() * r_operand);
}
template<class Vector, typename T>
constexpr inline Vector operator/(const pathPoint<T>& l_operand, const T& r_operand) {
	return Vector(l_operand.x() / r_operand, l_operand.y() / r_operand);
}

template<typename T, class Vector>
constexpr inline Vector operator+(const pathPoint<T>& l_operand, const Vector& r_operand) {
	return Vector(l_operand.x() + r_operand.x(), l_operand.y() + r_operand.x());
}
template<typename T, class Vector>
constexpr inline Vector operator+(const Vector& l_operand, const pathPoint<T>& r_operand) {
	return Vector(l_operand.x() + r_operand.x(), l_operand.y() + r_operand.x());
}
template<typename T, class Vector>
constexpr inline Vector operator-(const pathPoint<T>& l_operand, const Vector& r_operand) {
	return Vector(l_operand.x() - r_operand.x(), l_operand.y() - r_operand.x());
}
template<typename T, class Vector>
constexpr inline Vector operator-(const Vector& l_operand, const pathPoint<T>& r_operand) {
	return Vector(l_operand.x() - r_operand.x(), l_operand.y() - r_operand.x());
}
