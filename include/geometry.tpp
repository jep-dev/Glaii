#ifndef GEOMETRY_TPP
#define GEOMETRY_TPP

namespace Geometry {
	template<typename X>
	X magnitude2(X const& x) {
		return dot(x, x);
	}
	template<typename X>
	X magnitude(X const& x) {
		return X(sqrt(magnitude2(x)));
	}
	template<typename X>
	X normalize(X const& x) {
		return x / magnitude(x);
	}

	template<typename X>
	X Vec_t<X>::operator[](unsigned char r) const {
		switch(r) {
			case 'i': case 'x': return x;
			case 'j': case 'y': return y;
			case 'k': case 'z': return z;
			default: return 0;
		}
	}
	/*template<typename X>
	X Vec_t<X>::magnitude2(void) const {
		return dot(*this, *this);
		//return x*x + y*y + z*z;
	}
	template<typename X>
	X Vec_t<X>::magnitude(void) const {
		return X(sqrt(magnitude2()));
	}
	template<typename X>
	Vec_t<X> Vec_t<X>::normalize(void) const {
		return *this / magnitude();
	}*/
	template<typename X>
	template<typename R, typename XR>
	Vec_t<XR> Vec_t<X>::operator-(Vec_t<R> const& r) const {
		return {x-r.x, y-r.y, z-r.z};
	}
	template<typename X>
	template<typename R>
	bool Vec_t<X>::operator==(Vec_t<R> const& r) const {
		return x == r.x && y == r.y && z == r.z;
	}
	template<typename X>
	template<typename R, typename XR>
	Vec_t<XR> Vec_t<X>::operator/(R const& r) const {
		return {x/r, y/r, z/r};
	}

	template<typename R, typename S, typename T,
		typename RS, typename ST, typename RST>
	Vec_t<RST> cross(Vec_t<R> const& l,
			Vec_t<S> const& c, Vec_t<T> const& r) {
		return (l-c)*(r-c);
	}
	template<typename L, typename R = L,
		typename LR = COMBINE(L,*,R)>
	Vec_t<LR> dot(Vec_t<L> const& l, Vec_t<R> const& r) {
		return l.x*r.x + l.y*r.y + l.z*r.z;
	}


	template<typename L, typename R, typename LR>
	Vec_t<LR> operator*(Vec_t<L> const& l, Vec_t<R> const& r) {
		return {LR(l.y * r.z - l.z * r.y),
			LR(l.z * r.x - l.x * r.z),
			LR(l.x * r.y - l.y * r.x)};
	}
	template<typename L, typename R, typename LR>
	Vec_t<LR> operator*(Vec_t<R> const& l, L const& r) {
		return {l.x*r, l.y*r, l.z*r};
	}
	template<typename L, typename R, typename LR>
	Vec_t<LR> operator*(L const& l, Vec_t<R> const& r) {
		return r * l;
	}

	/**
	 * @brief Fuzzy comparison to zero (avoids division due to aliasing)
	 * @tparam L The domain of the value to compare
	 * @param l The subject to compare to the range endpoints
	 * @param prox The proximity or tolerance factor
	 * @return True if and only if the value is within [-1,1] after
	 * multiplication with the given or default proximity
	 */
	template<typename L>
	bool nearZero(L const& l, unsigned prox) {
		auto lp = l * prox;
		return lp >= -1 && lp <= 1;
	}
	/**
	 * @brief Rounds to zero by fuzzy comparison
	 * @tparam T The domain of the value to round
	 * @param t The subject to round
	 * @param prox The proximity or tolerance factor
	 * @return A copy of t, or zero if t is within prox of zero
	 */
	template<typename T>
	T roundNearZero(T const& t, unsigned prox) {
		return nearZero(t, prox) ? T(0) : t;
	}
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
	bool near(L const& l, R const& r, unsigned prox) {
		return nearZero(magnitude(r-l), prox);
	}
}

#endif
