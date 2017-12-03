#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "geometry.hpp"

namespace Geometry {
	/**
	 * @brief Quaternion template with format i, j, k, real.
	 * @tparam X The domain of each member.
	 */
	template<typename X>
	struct Quat_t {
		X w, x, y, z;
		template<typename R>
		Quat_t<X> operator=(Quat_t<R> const& r) {
			w = X(r.w); x = X(r.x);
			y = X(r.y); z = X(r.z);
			return *this;
		}
		Quat_t<X> operator*(void) const {
			return {w, -x, -y, -z};
		}
		Quat_t<X> operator-(void) const {
			return {-w, -x, -y, -z};
		}
		X magnitude2(void) const {
			return w*w + x*x + y*y + z*z;
		}
		X magnitude(void) const {
			return X(sqrt(magnitude2()));
		}
		Quat_t<X> normalize(void) const {
			return *this / magnitude();
		}
		template<typename R, typename XR = COMBINE(X,+,R)>
		Quat_t<XR> operator+(Quat_t<R> const& r) const {
			return {XR(w+r.w), XR(x+r.x), XR(y+r.y), XR(z+r.z)};
		}
		template<typename R, typename XR = COMBINE(X,-,R)>
		Quat_t<XR> operator-(Quat_t<R> const& r) const {
			return {XR(w-r.w), XR(x-r.x), XR(y-r.y), XR(z-r.z)};
		}
		template<typename R>
		bool operator==(Quat_t<R> const& r) const {
			return w == r.w && x == r.x
				&& y == r.y && z == r.z;
		}
		template<typename R, typename XR = COMBINE(X,/,R)>
		Quat_t<XR> operator/(R const& r) const {
			return {w/r, x/r, y/r, z/r};
		}
	};
	/**
	 * @brief Angle-axis rotation to equivalent quaternion
	 * @tparam L The domain of the rotation magnitude
	 * @tparam R The domain of the rotation axis
	 * @param l The rotation angle in radians
	 * @param r The rotation axis, generally normalized
	 * @tparam LR The range of the resulting quaternion
	 * @return The quaternion representation of the rotation
	 */
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Quat_t<LR> rotate(L const& l, Vec_t<R> const& r) {
		LR l2 = LR(l)/2, lc = LR(cos(l2)), ls = LR(sin(l2));
		return Quat_t<LR>{lc, ls*r.x, ls*r.y, ls*r.z};
	}

	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Quat_t<LR> operator*(Quat_t<L> const& l, Quat_t<R> const& r) {
		return {
			LR(l.w * r.w - l.x * r.x - l.y * r.y - l.z * r.z),
			LR(l.w * r.x + l.x * r.w + l.y * r.z - l.z * r.y),
			LR(l.w * r.y - l.x * r.z + l.y * r.w + l.z * r.x),
			LR(l.w * r.z + l.x * r.y - l.y * r.x + l.z * r.w)
		};
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Quat_t<LR> operator*(Quat_t<L> const& l, Vec_t<R> const& r) {
		return Quat_t<LR> {
			LR(          - l.x * r.x - l.y * r.y - l.z * r.z),
			LR(l.w * r.x             + l.y * r.z - l.z * r.y),
			LR(l.w * r.y - l.x * r.z             + l.z * r.x),
			LR(l.w * r.z + l.x * r.y - l.y * r.x            )
		}**l;
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Quat_t<LR> operator*(Quat_t<R> const& l, R const& r) {
		return {LR(l.w*r), LR(l.x*r), LR(l.y*r), LR(l.z*r)};
	}
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Quat_t<LR> operator*(L const& l, Quat_t<R> const& r) {
		return r*l;
	}

	template struct Quat_t<float>;
	template struct Quat_t<double>;
}

#endif
