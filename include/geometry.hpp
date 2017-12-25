/*! @file include/geometry.hpp
 *  @brief Root for quaternions, dual quaternions, and other geometry */

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "abstract.hpp"

///@cond
#include <math.h>
///@endcond

#ifndef TOLERANCE
#define TOLERANCE 512
#endif

namespace Geometry {

	/**
	 * @brief A 3D vector template, analogous to pure-imaginary quaternions.
	 * @tparam X The domain of each of the members.
	 */
	template<typename> struct Vec_t;

	template<typename> struct Quat_t;
	template<typename> struct DualQuat_t;

	template<typename X>
	struct Vec_t {
		X x, y, z;
		X operator[](unsigned char r) const;

		//X magnitude2(void) const;
		//X magnitude(void) const;
		//Vec_t<X> normalize(void) const;

		template<typename R>
		bool operator==(Vec_t<R> const& r) const;

		template<typename R, typename XR = COMBINE(X,-,R)>
		Vec_t<XR> operator-(Vec_t<R> const& r) const;

		template<typename R, typename XR = COMBINE(X,/,R)>
		Vec_t<XR> operator/(R const& r) const;
	};

	template<typename X>
	X magnitude2(X const& x);
	template<typename X>
	X magnitude(X const& x);
	template<typename X>
	X normalize(X const& x);

	/**
	 * @brief The cross product of two (un-normalized) vector differences.
	 * @tparam R The domain of the first vector
	 * @tparam S The domain of the second vector, the shared origin
	 * @tparam T The domain of the third vector
	 * @tparam RS The range of the first vector difference
	 * @tparam ST The range of the second vector difference
	 * @tparam RST The range of the final cross product
	 * @return The resulting cross product
	 */
	template<typename R, typename S = R, typename T = S,
		typename RS = COMBINE(R,-,S), typename ST = COMBINE(T,-,S),
		typename RST = COMBINE(RS,*,ST)>
	Vec_t<RST> cross(Vec_t<R> const& l, Vec_t<S> const& c, Vec_t<T> const& r);

	/**
	 * @brief The dot or inner product of two vectors.
	 * @tparam R The domain of the first vector
	 * @tparam S The domain of the second vector
	 * @tparam RS The range of the scalar product
	 * @return The resulting dot/inner product
	 */
	template<typename L, typename R = L,
		typename LR = COMBINE(L,*,R)>
	Vec_t<LR> dot(Vec_t<L> const& l, Vec_t<R> const& r);

	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Vec_t<LR> operator*(Vec_t<L> const& l, Vec_t<R> const& r);
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Vec_t<LR> operator*(Vec_t<R> const& l, L const& r);
	template<typename L, typename R, typename LR = COMBINE(L,*,R)>
	Vec_t<LR> operator*(L const& l, Vec_t<R> const& r);

	/**
	 * @brief Fuzzy comparison to zero (avoids division due to aliasing)
	 * @tparam L The domain of the value to compare
	 * @param l The subject to compare to the range endpoints
	 * @param prox The proximity or tolerance factor
	 * @return True if and only if the value is within [-1,1] after
	 * multiplication with the given or default proximity
	 */
	template<typename L>
	bool nearZero(L const& l, unsigned prox = TOLERANCE);
	/**
	 * @brief Rounds to zero by fuzzy comparison
	 * @tparam T The domain of the value to round
	 * @param t The subject to round
	 * @param prox The proximity or tolerance factor
	 * @return A copy of t, or zero if t is within prox of zero
	 */
	template<typename T>
	T roundNearZero(T const& t, unsigned prox = TOLERANCE);
	/**
	 * @brief Fuzzy comparison of multidimensional values
	 * @tparam L Type of the subject with magnitude method
	 * @tparam R Type of the object with magnitude method
	 * @param l The subject or subtrahend of the comparison
	 * @param r The object or minuend of the comparison
	 * @param prox The proximity or tolerance factor
	 * @return True if and only if the magnitude of the difference of
	 * inputs is near zero
	 */
	template<typename L, typename R>
	bool near(L const& l, R const& r, unsigned prox = TOLERANCE);

	// Macros if this starts to get tedious?
	template struct Vec_t<float>;
	template struct Vec_t<double>;

}
#include "geometry.tpp"

#endif
