#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "geometry.hpp"


namespace Geometry {

	template<typename X = float>
	struct Quat_t;

	using Quatf = Quat_t<float>;
	using Quatd = Quat_t<double>;

	/// Enables a constant literal notation of quaternions (until compiler
	/// support catches up for template argument deduction over operator"").
	struct Unit {
		/* This run-time "unit" identified by e and the planned
		 * strongly-typed hypercomplex unit types are unrelated. */
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
		template<typename R, typename XR = COMBINE(X,*,R)>
		Quat_t<XR> operator*(Quat_t<R> const& r) const;
		/// @brief The 'conjugacy' or 'sandwich product', A^B := ABA*.
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
	Quat_t<LR> rotation(L const& l, Vec_t<R> const& r);

	// User-defined suffixes, e.g. Quatf x = 1.0_j*1.0_k
	// TODO template this; DRY, esp. anticipating ad-hoc hypercomplex
	// (See note on 'Unit' helper type)
	Unit operator"" _r(long double v) { return {0,v}; }
	Unit operator"" _i(long double v) { return {1,v}; }
	Unit operator"" _j(long double v) { return {2,v}; }
	Unit operator"" _k(long double v) { return {3,v}; }

}
#include "quaternion.tpp"

#endif
