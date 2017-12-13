#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "geometry.hpp"


namespace Geometry {

	template<typename X = float>
	struct Quat_t;

	using Quatf = Quat_t<float>;
	using Quatd = Quat_t<double>;

	/*
	 * Unit is a helper type bridging suffixes to quaternions (and future
	 * unital algebras) - only necessary due to compiler shortcomings!
	 *
	 * The compiler selects (deduces) Quat_t<X> via implicit conversion
	 * operators in Unit, but prohibits use of types dependent on deduced
	 * X - like Quat_t<X> - without a helper providing these conversions;
	 * lift-reduction (kappa calculus) should preclude the need for Unit.
	 * TODO track; circumvent before design of unified hypercomplex.
	 */
	struct Unit {
		// u in {0,1,2,3} -> {1,i,j,k}
		unsigned e;
		long double v;
		operator Quatf(void) const;
		operator Quatd(void) const;
	};

	/**
	 * @brief Quaternion template with format i, j, k, real.
	 * @tparam X The domain of each member.
	 */
	template<typename X>
	struct Quat_t {
		X w, x, y, z;

		friend Unit operator"" _r();
		friend Unit operator"" _i();
		friend Unit operator"" _j();
		friend Unit operator"" _k();

		X operator[](unsigned i) const {
			return (i==0) ? w : (i==1) ? x : (i==2) ? y : (i==3) ? z : X(0);
		}
		X operator[](char c) const {
			return c == '1' ? w : c == 'i' ? x : c == 'j'
				? y : c == 'k' ? z : 0;
		}
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
		template<typename R, typename XR = COMBINE(X,*,R)>
		Quat_t<XR> operator*(Quat_t<R> const& r) const {
			return {
				XR(w * r.w - x * r.x - y * r.y - z * r.z),
				XR(w * r.x + x * r.w + y * r.z - z * r.y),
				XR(w * r.y - x * r.z + y * r.w + z * r.x),
				XR(w * r.z + x * r.y - y * r.x + z * r.w)
			};
		}
		/**
		 * @brief The 'conjugacy' or 'sandwich product', A^B := ABA*.
		 * TODO: profiling; inlining? (C=AB, D=CA* vs. D=ABA*)
		 */
		template<typename R, typename XR = COMBINE(X,*,R)>
		Quat_t<XR> operator^(Vec_t<R> const& r) const {
			return Quat_t<XR> {
				XR(          - x * r.x - y * r.y - z * r.z),
				XR(w * r.x             + y * r.z - z * r.y),
				XR(w * r.y - x * r.z             + z * r.x),
				XR(w * r.z + x * r.y - y * r.x            )
			}***this;
		}
		template<typename R, typename XR = COMBINE(X,*,R)>
		Quat_t<XR> operator^(Quat_t<R> const& r) const {
			return *this * r * * *this;
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
	Quat_t<LR> rotation(L const& l, Vec_t<R> const& r) {
		LR l2 = LR(l)/2, lc = LR(cos(l2)), ls = LR(sin(l2));
		return Quat_t<LR>{lc, ls*r.x, ls*r.y, ls*r.z};
	}

	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Quat_t<LR> operator*(L const& l, Quat_t<R> const& r) {
		return {LR(l*r.w), LR(l*r.x), LR(l*r.y), LR(l*r.z)};
	}


	// User-defined suffixes, e.g. Quatf x = 1.0_j*1.0_k
	// TODO template this; DRY, esp. anticipating ad-hoc hypercomplex
	// (See note on 'Unit' helper type)
	Unit operator"" _r(long double v) { return {0,v}; }
	Unit operator"" _i(long double v) { return {1,v}; }
	Unit operator"" _j(long double v) { return {2,v}; }
	Unit operator"" _k(long double v) { return {3,v}; }

	Unit::operator Quatf(void) const {
		float rijk[4] = { 0 }; rijk[e] = float(v);
		return {rijk[0], rijk[1], rijk[2], rijk[3]};
	}
	Unit::operator Quatd(void) const {
		double rijk[4] = { 0 }; rijk[e] = double(v);
		return {rijk[0], rijk[1], rijk[2], rijk[3]};
	}

}

#endif
