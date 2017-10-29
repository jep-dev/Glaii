#include "streams.hpp"
#include "cutter.hpp"

namespace Streams {
	Cutter::operator bool(void) const {
		return data.size();
	}
	Cutter::operator string const&(void) const {
		return data;
	}
	Cutter::operator const char*(void) const {
		return data.c_str();
	}
	Cutter::Cutter(void) {}
	Cutter::Cutter(string const& s): data(s) {}
	Cutter::Cutter(string && s): data(std::move(s)) {}
	Cutter::Cutter(const char *fname):
		Cutter(ifstream(fname)) {}
}
