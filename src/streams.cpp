#include "streams.hpp"

namespace Streams {
	Cutter::operator string const&(void) const {
		return data;
	}
	Cutter::operator const char*(void) const {
		return data.c_str();
	}

	Cutter::Cutter(void) {}
	Cutter::Cutter(string const& s): data(s) {}
	Cutter::Cutter(string && s): data(std::move(s)) {}
	template<typename S>
	Cutter::Cutter(S && s):
		data(std::istreambuf_iterator<char>(s), {}) {}
	Cutter::Cutter(const char *fname):
		Cutter(ifstream(fname)) {}
}
