/*! @file include/streams.hpp
 *  @brief Root for IO, including iostream, 'Cutter' and 'Paster' */

#ifndef STREAMS_HPP
#define STREAMS_HPP

#include "geometry.hpp"
#include "matrix.hpp"

///@cond
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <functional>
///@endcond

namespace Streams {
	using std::size_t;
	using std::string;
	using std::ostream;
	using std::ifstream;
	using std::basic_streambuf;
	using std::ostringstream;

	template<typename S, typename... T>
	S repeat(S && dest, T const&... tail) {
		return std::move(dest);
	}
	template<typename S, typename H, typename HN, typename... T>
	S repeat(S && dest, H const& head, HN const& reps, T const&... tail) {
		for(auto i = 0; i < reps; i++) dest << head << '\n';
		return repeat(std::move(dest), tail...);
	}
	template<typename S, typename H, typename... T>
	S repeat(S && dest, H const& head, T const&... tail) {
		dest << head;
		return repeat(std::move(dest), tail...);
	}

	template<typename S, typename... T>
	S column(S && dest, T const&... tail) {
		return std::move(dest);
	}
	template<typename S, typename H, typename... T>
	S column(S && dest, H const& head, T const&... tail) {
		dest << head << '\n';
		return column(std::move(dest), tail...);
	}

	struct Cutter;
	struct Paster;

}

#include "cutter.hpp"
#include "paster.hpp"

namespace Geometry {
	template<typename S, typename T>
	S& operator<<(S& dest, Quat_t<T> const& src);
	template<typename S, typename T>
	S& operator<<(S& dest, DualQuat_t<T> const& src);
	template<typename S, typename T>
	S& operator<<(S& dest, Vec_t<T> const& src);
	template<typename S, typename T>
	S& operator<<(S& dest, Matrix_t<T> const& m);
}
#include "streams.tpp"

#endif
