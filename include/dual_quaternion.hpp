#ifndef DUAL_QUATERNION_HPP
#define DUAL_QUATERNION_HPP

#include "geometry.hpp"
#include "quaternion.hpp"

namespace Geometry {
	template<typename X>
	struct DualQuat_t {
		Quat_t<X> u = {0,0,0,0}, v = {0,0,0,0};
		Quat_t<X> operator[](unsigned i) const {
			return i ? v : u;
		}
		template<typename R>
		DualQuat_t<X> operator=(DualQuat_t<R> const& r) {
			u = r.u; v = r.v;
			return *this;
		}
		DualQuat_t<X> operator*(void) const {
			return {*u, *v};
		}
		DualQuat_t<X> operator-(void) const {
			return {-u, -v};
		}
		X magnitude2(void) const {
			return u.magnitude2() + v.magnitude2();
		}
		X magnitude(void) const {
			return X(sqrt(magnitude2()));
		}
		DualQuat_t<X> normalize(void) const {
			return *this / magnitude();
		}
		template<typename R, typename XR = COMBINE(X,+,R)>
		DualQuat_t<XR> operator+(DualQuat_t<R> const& r) const {
			return {u + r.u, v + r.v};
		}
		template<typename R, typename XR = COMBINE(X,-,R)>
		DualQuat_t<XR> operator-(DualQuat_t<R> const& r) const {
			return {u - r.u, v - r.v};
		}
		template<typename R>
		bool operator==(DualQuat_t<R> const& r) const {
			return u == r.u && v == r.v;
		}
		template<typename R, typename XR = COMBINE(X,/,R)>
		DualQuat_t<XR> operator/(R const& r) const {
			return {u/r, v/r};
		}
	};

	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	DualQuat_t<LR> operator*(DualQuat_t<L> const& l, DualQuat_t<R> const& r) {
		return {l.u*r.u, l.u*r.v + l.v*r.u};
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	DualQuat_t<LR> operator*(DualQuat_t<L> const& l, Quat_t<R> const& r) {
		return {l.u*r, l.v*r};
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	DualQuat_t<LR> operator*(L const& l, DualQuat_t<R> const& r) {
		return {l*r.u, l*r.v};
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	DualQuat_t<LR> operator^(DualQuat_t<L> const& l, DualQuat_t<R> const& r) {
		return DualQuat_t<LR>{l.u*r.u, l.u*r.v + l.v*r.u} * *l;
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	DualQuat_t<LR> operator^(DualQuat_t<L> const& l, Quat_t<R> const& r) {
		return DualQuat_t<LR>{l.u*r, l.v*r} * *l;
	}

	template struct DualQuat_t<float>;
	template struct DualQuat_t<double>;
}

#endif
