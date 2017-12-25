/*! @file include/dual_quaternion.hpp
 *  @brief Extends quaternions' rotations, introducing axes at infinity */

#ifndef DUAL_QUATERNION_HPP
#define DUAL_QUATERNION_HPP

#include "geometry.hpp"
#include "quaternion.hpp"

namespace Geometry {
	template<typename X>
	struct DualQuat_t {
		Quat_t<X> u = {0,0,0,0}, v = {0,0,0,0};
		Quat_t<X> operator[](unsigned i) const;
		template<typename R>
		DualQuat_t<X> operator=(DualQuat_t<R> const& r);
		DualQuat_t<X> operator*(void) const;
		DualQuat_t<X> operator-(void) const;
		template<typename R, typename XR = COMBINE(X,+,R)>
		DualQuat_t<XR> operator+(DualQuat_t<R> const& r) const;
		template<typename R, typename XR = COMBINE(X,-,R)>
		DualQuat_t<XR> operator-(DualQuat_t<R> const& r) const;
		template<typename R>
		bool operator==(DualQuat_t<R> const& r) const;
		template<typename R, typename XR = COMBINE(X,/,R)>
		DualQuat_t<XR> operator/(R const& r) const;
	};

	template<typename L, typename R = L, typename LR = COMBINE(L,*,R)>
	LR dot(DualQuat_t<L> const& l, DualQuat_t<R> const& r);
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	DualQuat_t<LR> operator*(DualQuat_t<L> const& l, DualQuat_t<R> const& r);
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	DualQuat_t<LR> operator*(DualQuat_t<L> const& l, Quat_t<R> const& r);
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	DualQuat_t<LR> operator*(L const& l, DualQuat_t<R> const& r);
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	DualQuat_t<LR> operator^(DualQuat_t<L> const& l, DualQuat_t<R> const& r);
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	DualQuat_t<LR> operator^(DualQuat_t<L> const& l, Quat_t<R> const& r);

	template struct DualQuat_t<float>;
	template struct DualQuat_t<double>;
}
#include "dual_quaternion.tpp"

#endif
