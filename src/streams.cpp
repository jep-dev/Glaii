#include "streams.hpp"

namespace Streams {
	Source::operator const string(void) const
		{ return data; }
	Source::operator const char*(void) const
		{ return data.c_str(); }

	Source::Source(void) {}
	Source::Source(string const& s): data(s) {}
	Source::Source(string && s): data(std::move(s)) {}
	template<typename S>
	Source::Source(S s): data((isbuf_it(s)), isbuf_it()) {}
	Source::Source(const char *fname): Source(ifstream(fname)) {}
}
