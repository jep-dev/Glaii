#ifndef STREAMS_HPP
#define STREAMS_HPP

#include <iosfwd>
#include <fstream>
#include <sstream>
#include <iosfwd>
#include <fstream>

namespace Streams {
	using isbuf_it = std::istreambuf_iterator<char>;
	using std::string;

	using std::ifstream;
	using std::ostream;
	using std::ostringstream;

	/** @brief Reads entire ifstream into memory. */
	struct Source {
		const string data;
		operator const char*(void) const;
		operator const string(void) const;
		Source(void);
		Source(string && s);
		Source(string const& s);
		Source(const char *fname);
		template<typename S> Source(S s);
	};
}

#endif
