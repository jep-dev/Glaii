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

		X operator[](unsigned i) const;
		X operator[](char c) const;
		template<typename R>
		Quat_t<X> operator=(Quat_t<R> const& r);
		Quat_t<X> operator*(void) const;
		Quat_t<X> operator-(void) const;
		/*X magnitude2(void) const;
		X magnitude(void) const;
		Quat_t<X> normalize(void) const;*/
		template<typename R, typename XR = COMBINE(X,*,R)>
		Quat_t<XR> operator*(Quat_t<R> const& r) const;
		/**
		 * @brief The 'conjugacy' or 'sandwich product', A^B := ABA*.
		 * TODO: profiling; inlining? (C=AB, D=CA* vs. D=ABA*)
		 */
		template<typename R, typename XR = COMBINE(X,*,R)>
		Quat_t<XR> operator^(Vec_t<R> const& r) const;
		template<typename R, typename XR = COMBINE(X,*,R)>
		Quat_t<XR> operator^(Quat_t<R> const& r) const;
		template<typename R, typename XR = COMBINE(X,+,R)>
		Quat_t<XR> operator+(Quat_t<R> const& r) const;
		template<typename R, typename XR = COMBINE(X,-,R)>
		Quat_t<XR> operator-(Quat_t<R> const& r) const;
		template<typename R>
		bool operator==(Quat_t<R> const& r) const;
		template<typename R, typename XR = COMBINE(X,/,R)>
		Quat_t<XR> operator/(R const& r) const;
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
#include "quaternion.tpp"

#endif
