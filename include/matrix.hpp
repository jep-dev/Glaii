/*! @file include/matrix.hpp
 *  @brief Defines 4x4 matrix types with basic operations */

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <abstract.hpp>

namespace Geometry {

	template<typename S = float>
	struct Matrix_t;

	template<typename S>
	struct Matrix_t {
		S data[16];
		static Matrix_t<S> identity(void) {
			return {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}
		Matrix_t<S> transpose(void) const {
			return {
				data[ 0], data[ 4], data[ 8], data[12],
				data[ 1], data[ 5], data[ 9], data[13],
				data[ 2], data[ 6], data[10], data[14],
				data[ 3], data[ 7], data[11], data[15]
			};
		}
		S const& operator[](unsigned N) const {
			return data[N];
		}
		S& operator[](unsigned N) {
			return data[N];
		}
		template<typename T>
		Matrix_t<S> operator=(Matrix_t<T> const& r) {
			std::copy(&r[0], &r[0]+16, data);
			return *this;
		}
		template<typename T>
		Matrix_t<COMBINE(S,*,T)> operator*(Matrix_t<T> const& r) {
			return {
				data[ 0]*r[ 0] + data[ 1]*r[ 4]
					+ data[ 2]*r[ 8] + data[ 3]*r[12],
				data[ 0]*r[ 1] + data[ 1]*r[ 5]
					+ data[ 2]*r[ 9] + data[ 3]*r[13],
				data[ 0]*r[ 2] + data[ 1]*r[ 6]
					+ data[ 2]*r[10] + data[ 3]*r[14],
				data[ 0]*r[ 3] + data[ 1]*r[ 7]
					+ data[ 2]*r[11] + data[ 3]*r[15],

				data[ 4]*r[ 0] + data[ 5]*r[ 4]
					+ data[ 6]*r[ 8] + data[ 7]*r[12],
				data[ 4]*r[ 1] + data[ 5]*r[ 5]
					+ data[ 6]*r[ 9] + data[ 7]*r[13],
				data[ 4]*r[ 2] + data[ 5]*r[ 6]
					+ data[ 6]*r[10] + data[ 7]*r[14],
				data[ 4]*r[ 3] + data[ 5]*r[ 7]
					+ data[ 6]*r[11] + data[ 7]*r[15],

				data[ 8]*r[ 0] + data[ 9]*r[ 4]
					+ data[10]*r[ 8] + data[11]*r[12],
				data[ 8]*r[ 1] + data[ 9]*r[ 5]
					+ data[10]*r[ 9] + data[11]*r[13],
				data[ 8]*r[ 2] + data[ 9]*r[ 6]
					+ data[10]*r[10] + data[11]*r[14],
				data[ 8]*r[ 3] + data[ 9]*r[ 7]
					+ data[10]*r[11] + data[11]*r[15],

				data[12]*r[ 0] + data[13]*r[ 4]
					+ data[14]*r[ 8] + data[15]*r[12],
				data[12]*r[ 1] + data[13]*r[ 5]
					+ data[14]*r[ 9] + data[15]*r[13],
				data[12]*r[ 2] + data[13]*r[ 6]
					+ data[14]*r[10] + data[15]*r[14],
				data[12]*r[ 3] + data[13]*r[ 7]
					+ data[14]*r[11] + data[15]*r[15],
			};
		}
	};
}

#endif
