#ifndef STREAMS_HPP
#define STREAMS_HPP

#include <algorithm>
#include <iosfwd>
#include <fstream>
#include <sstream>
#include <iosfwd>
#include <fstream>
#include <numeric>
#include <map>
#include <vector>

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

	std::vector<std::string>
	split(std::string const& src, unsigned srcWidth, char delim = '\n') {
		std::string buf, srcStr = src;
		std::vector<std::string> lines;
		std::stringstream ss(srcStr);
		//while(getline(ss, buf, delim)) {
		do {
			bool done = !getline(ss, buf, delim);
			unsigned buf_len = buf.size(),
					buf_rem = buf_len % srcWidth;
			if(buf_rem) buf += std::string(srcWidth - buf_rem, ' ');
			lines.emplace_back(buf);
			if(done || !ss.good()) break;
		} while(1);
		return lines;
	}
	std::ostream& paste(std::ostream& dest,
			std::string const& lhs, unsigned lWidth,
			std::string const& rhs, unsigned rWidth,
			unsigned sepWidth = 3, char delim = '\n') {
		char div0 = '|', div1 = '|';
		auto lSplit = split(lhs, lWidth, delim),
			rSplit = split(rhs, rWidth, delim);
		unsigned lHeight = lSplit.size(), rHeight = rSplit.size(),
			maxHeight = std::max(lHeight, rHeight);
		if(!maxHeight) return dest;
		lSplit.reserve(maxHeight);
		rSplit.reserve(maxHeight);
		if(lHeight < maxHeight) {
			std::fill_n(&lSplit[lHeight], maxHeight - lHeight, "");
		}
		for(unsigned i = 0; i < maxHeight; i++) {
			std::string lStr, rStr;
			if(i < lHeight) lStr = lSplit[i];
			if(i < rHeight) rStr = rSplit[i];
			unsigned lSize = lStr.size(), rSize = rStr.size(),
					lLines = lSize / lWidth, rLines = rSize / rWidth,
					maxLines = std::max(lLines, rLines),
					ldLines = maxLines - lLines,
					rdLines = maxLines - rLines;
			if(lLines < maxLines)
				lStr += std::string(ldLines*lWidth, ' ');
			else if(rLines < lLines)
				rStr += std::string(rdLines*rWidth, ' ');
			for(unsigned j = 0; j < maxLines; j++) {
				dest << lStr.substr(j*lWidth, lWidth);
				if(sepWidth) {
					std::string sepStr(sepWidth, ' ');
					sepStr[sepWidth - 1 - int(sepWidth/2)] =
						(j ? div0 : div1);
					dest << sepStr;
				}
				dest << rStr.substr(j*rWidth, rWidth) << '\n';
			}
		}
		return dest;
	}
	std::ostream&
	pasteBorder(std::ostream& dest,
			std::string const& lhs, unsigned lWidth,
			std::string const& rhs, unsigned rWidth,
			unsigned sepWidth = 3, unsigned bordWidth = 1,
			bool north = true, bool south = true,
			char delim = '\n') {
		std::ostringstream oss;
		paste(oss, lhs, lWidth, rhs, rWidth, sepWidth, delim);
		if(!bordWidth) return dest << oss.str();
		std::string src = oss.str(), buf, border(bordWidth, ' ');
		border[0] = '|';
		std::stringstream bufRd(src);
		unsigned j = 0, jw = bordWidth*2 + sepWidth + lWidth + rWidth;
		if(north) dest << '.' << std::string(jw-2, '-') << '.' << '\n';
		while(getline(bufRd, buf, delim)) {
			dest << border << buf;
			std::swap(border[0], border[bordWidth-1]);
			dest << border << '\n';
			std::swap(border[0], border[bordWidth-1]);
		}
		if(south) dest << '\'' << std::string(jw-2, '-') << '\'' << '\n';
		return dest;
	}
}

#endif
