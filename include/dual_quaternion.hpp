#ifndef DUAL_QUATERNION_HPP
#define DUAL_QUATERNION_HPP

#include "geometry.hpp"
#include "quaternion.hpp"

namespace Geometry {
	/* This would be a pair of quaternions, but the pure dual is never used
	 * in representing scale, rotation, or translation, so 7 values are
	 * used instead of 8. Two rows of 4 and two diagonals of 3 are removed
	 * from the product of dual quaternions, so 14 of 48 operations are
	 * eliminated! */
	template<typename X>
	struct DualQuat_t {
		X w = 0, x = 0, y = 0, z = 0, ex = 0, ey = 0, ez = 0;
		template<typename R>
		DualQuat_t<X> operator=(DualQuat_t<R> const& r) {
			x = r.x; y = r.y; z = r.z; w = r.w;
			ex = r.ex; ey = r.ey; ez = r.ez;
			return *this;
		}
		DualQuat_t<X> operator*(void) const {
			return {
				w, -x, -y, -z, -ex, -ey, -ez
			};
		}
		DualQuat_t<X> operator-(void) const {
			return {-w, -x, -y, -z, -ex, -ey, -ez};
		}
		X magnitude2(void) const {
			return w*w + x*x + y*y + z*z + ex*ex + ey*ey + ez*ez;
		}
		X magnitude(void) const {
			return X(sqrt(magnitude2()));
		}
		DualQuat_t<X> normalize(void) const {
			return *this / magnitude();
		}
		template<typename R, typename XR = COMBINE(X,*,R)>
		DualQuat_t<XR> operator*(DualQuat_t<R> const& r) const {
			return {
				w*r.w - x*r.x - y*r.y - z*r.z,
				w*r.x + x*r.w + y*r.z - z*r.y,
				w*r.y - x*r.z + y*r.w + z*r.x,
				w*r.z + x*r.y - y*r.x + z*r.w,
				w*r.ex + y*r.ez - z*r.ey
					+ ex*r.w + ey*r.z - ez*r.y,
				w*r.ey - x*r.ez + z*r.ex
					- ex*r.z + ey*r.w + ez*r.x,
				w*r.ez + x*r.ey - y*r.ex
					+ ex*r.y - ey*r.x + ez*r.w
			};
		}
		template<typename R, typename XR = COMBINE(X,+,R)>
		DualQuat_t<XR> operator+(DualQuat_t<R> const& r) const {
			return {w + r.w, x + r.x, y + r.y, z + r.z,
				ex + r.ex, ey + r.ey, ez + r.ez};
		}
		template<typename R, typename XR = COMBINE(X,-,R)>
		DualQuat_t<XR> operator-(DualQuat_t<R> const& r) const {
			return {w - r.w, x - r.x, y - r.y, z - r.z,
				ex - r.ex, ey - r.ey, ez - r.ez};
		}
		template<typename R>
		bool operator==(DualQuat_t<R> const& r) const {
			return w == r.w && x == r.x && y == r.y && z == r.z
				&& ex == r.ex && ey == r.ey && ez == r.ez;
		}
		template<typename R, typename XR = COMBINE(X,/,R)>
		DualQuat_t<XR> operator/(R const& r) const {
			return {w/r, x/r, y/r, z/r, ex/r, ey/r, ez/r};
		}
	};

	template struct DualQuat_t<float>;
	template struct DualQuat_t<double>;
}

#endif
