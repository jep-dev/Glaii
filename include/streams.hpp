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
	S& operator<<(S& dest, Vec_t<T> const& src) {
		Streams::Paster paster;
		paster.column(roundNearZero(src.x), roundNearZero(src.y),
				roundNearZero(src.z)).column('i','j','k');
		dest << paster;
		return dest;
	}
	template<typename S, typename T>
	S& operator<<(S& dest, Matrix_t<T> const& m) {
		Streams::Paster paster;
		for(auto i = 0; i < 4; i++)
			(paster << " ").column(roundNearZero(m[i]),
				roundNearZero(m[i+4]), roundNearZero(m[i+8]),
				roundNearZero(m[i+12])) << (i ? " " : "");
		dest << paster;
		return dest;
	}
}

#endif
