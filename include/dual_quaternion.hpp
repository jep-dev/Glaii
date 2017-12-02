#ifndef DUAL_QUATERNION_HPP
#define DUAL_QUATERNION_HPP

#include "geometry.hpp"
#include "quaternion.hpp"

namespace Geometry {
	template<typename X>
	struct DualQuat_t {
		Quat_t<X> c, d;
		template<typename R>
		DualQuat_t<X> operator=(DualQuat_t<R> const& r) {
			c = (Quat_t<X>) r.c;
			d = (Quat_t<X>) r.d;
			return *this;
		}
		DualQuat_t<X> operator*(void) const {
			return {*c, -*d};
		}
		DualQuat_t<X> operator-(void) const {
			return {-c, -d};
		}
		Quat_t<X> operator[](unsigned char r) const {
			switch(r) {
				case 'd': case 'e': return d;
				default: return c;
			}
		}
		X magnitude2(void) const {
			return X(c.magnitude2() + d.magnitude2());
		}
		X magnitude(void) const {
			return X(sqrt(magnitude2()));
		}
		DualQuat_t<X> normalize(void) const {
			return *this / magnitude();
		}
		template<typename R, typename XR = COMBINE(X,+,R)>
		DualQuat_t<XR> operator+(DualQuat_t<R> const& r) const {
			return {c + r.c, d + r.d};
		}
		template<typename R, typename XR = COMBINE(X,-,R)>
		DualQuat_t<XR> operator-(DualQuat_t<R> const& r) const {
			return {c - r.c, d - r.d};
		}
		template<typename R>
		bool operator==(DualQuat_t<R> const& r) const {
			return c == r.c && d == r.d;
		}
		template<typename R, typename XR = COMBINE(X,/,R)>
		DualQuat_t<XR> operator/(R const& r) const {
			return {c/r, d/r};
		}
	};

	template struct DualQuat_t<float>;
	template struct DualQuat_t<double>;
}

#endif
