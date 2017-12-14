#ifndef STREAMS_HPP
#define STREAMS_HPP

#include "geometry.hpp"
#include "matrix.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <functional>

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
	S& operator<<(S& dest, Quat_t<T> const& src) {
		dest << std::showpos;
		bool hit = false;
		T x[] = {src.w, src.x, src.y, src.z};
		const char *l[] = {"", "i", "j", "k"};
		for(unsigned i = 0; i < 4; i++) {
			auto ix = x[i];
			if(nearZero(ix)) continue;
			auto il = l[i];
			hit = true;
			auto ax = float(abs(ix));
			if(i && nearZero(ax - 1))
				dest << ((ix < 0) ? "-" : "+") << il;
			else dest << ix << il;
		}
		if(!hit) dest << T(0);
		dest << std::noshowpos;
		return dest;
	}
	template<typename S, typename T>
	S& operator<<(S& dest, DualQuat_t<T> const& src) {
		dest << std::showpos;

		bool hit = false;
		Quat_t<T> const& u = src.u, v = src.v;
		T x[] = {u.w, u.x, u.y, u.z, v.w, v.x, v.y, v.z};
		const char *l[] = {"", "i", "j", "k", "e", "ei", "ej", "ek"};
		for(unsigned i = 0; i < 8; i++) {
			auto ix = x[i];
			if(nearZero(ix)) continue;
			auto il = l[i];
			hit = true;
			auto ax = float(abs(ix));
			if(i && nearZero(ax - 1))
				dest << ((ix < 0) ? "-" : "+") << il;
			else dest << ix << il;
		}
		if(!hit) dest << T(0);
		dest << std::noshowpos;
		return dest;
	}
	template<typename S, typename T>
	S& operator<<(S& dest, Vec_t<T> const& src) {
		//Streams::Paster paster;
		using Streams::column;
		using std::ostringstream;
		ostringstream oss;
		oss << std::showpos;
		dest << (Streams::Paster() << column(std::move(oss),
				roundNearZero(src.x), roundNearZero(src.y),
				roundNearZero(src.z))
			<< column(ostringstream(), 'i','j','k'));
		//dest << paster;
		return dest;
	}
	template<typename S, typename T>
	S& operator<<(S& dest, Matrix_t<T> const& m) {
		Streams::Paster paster;
		for(auto i = 0; i < 4; i++) {
			if(i) paster << ' ';
			std::ostringstream oss;
			oss << std::showpos;
			paster << Streams::column(std::move(oss),
				roundNearZero(m[i]), roundNearZero(m[i+4]),
				roundNearZero(m[i+8]), roundNearZero(m[i+12]));
		}
		dest << paster;
		return dest;
	}
}

#endif
