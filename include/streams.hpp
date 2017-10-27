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
	using std::ostringstream;

	/** @brief Reads entire ifstream into memory. */
	struct Source {
		const string data;
		operator const char*(void) const;
		operator string const&(void) const;
		Source(void);
		Source(string && s);
		Source(string const& s);
		Source(const char *fname);
		template<typename S> Source(S && s);
	};

	struct Paster {
	protected:
		std::vector<string> data;
	public:
		bool pos = true;
		char pad = ' ';
		explicit operator string(void) const {
			ostringstream oss;
			oss << *this;
			return oss.str();
		}
		Paster& flush(size_t toCol = 0, size_t toRow = 0) {
			toCol = std::accumulate(data.cbegin(), data.cend(), toCol,
				[] (size_t lhs, string const& rhs) -> size_t {
					return std::max(lhs, rhs.size());
				});
			std::for_each(data.begin(), data.end(),
				[=] (string& src) -> string& {
				if(toCol > src.size())
					return src += string(toCol-src.size(), pad);
				});
			while(data.size() < toRow)
				data.emplace_back(toCol, pad);
			return *this;
		}
		Paster& operator<<(string const& rhs) {
			std::vector<string> sp;
			std::istringstream ss(rhs);
			while(ss) {
				string buf;
				if(getline(ss, buf, '\n')) {
					sp.emplace_back(buf);
				}
			}
			flush(0, sp.size());
			auto dBeg = data.begin();
			for(auto const& el : sp) {
				if(dBeg == data.end()) break;
				*dBeg += el;
				dBeg++;
			}
			return flush();
		}
		Paster& operator<<(ostringstream const& rhs) {
			return (*this) << rhs.str();
		}
		template<typename... T>
		Paster& column(T const&... t) {
			ostringstream oss;
			if(pos) oss << std::showpos;
			return column(oss, t...);
		}
		template<typename T0, typename... TN>
		Paster& column(ostringstream& oss, T0 const& t0, TN const&... tn) {
			oss << t0 << '\n';
			return column(oss, tn...);
		}
		template<typename... TN>
		Paster& column(ostringstream& oss, TN const&... tn) {
			return *this << oss;
		}
		template<typename T0, typename N = std::size_t, typename... TN>
		Paster& repeat(T0 const& t, N repeats, TN const&... tn) {
			ostringstream oss;
			if(pos) oss << std::showpos;
			return repeat(oss, t, repeats, tn...);
		}
		template<typename T0, typename N = std::size_t, typename... TN>
		Paster& repeat(ostringstream& dest,
				T0 const& t, N repeats, TN const&... tn) {
			for(auto i = 0; i < repeats; i++)
				dest << t << '\n';
			return repeat(dest, tn...);
		}
		Paster& repeat(ostringstream& dest) {
			return *this << dest;
		}
		template<typename T>
		Paster& center(T const& t) {
			ostringstream oss;
			auto cur = data.size(),
				centered = cur/2;
			string vpad(centered, '\n');
			oss << vpad << t;
			return *this << oss;
		}
		template<typename T>
		Paster& operator<<(T const& t) {
			ostringstream oss;
			oss << t;
			return *this << oss;
		}
		template<typename T>
		friend T& operator<<(T& dest, Paster const& src) {
			for(auto const& line : src.data) {
				dest << line << "\n";
			}
			return dest;
		}
		Paster(void) {}
		template<typename... T>
		Paster(T const&... t): Paster() {
			*this << string(t...);
		}
	};

	/*std::ostream& border(std::ostream& dest,
		string const& src, unsigned srcWidth,
		unsigned westWidth = 2, unsigned eastWidth = 2,
		bool north = true, bool south = true, char delim = '\n') {
		auto sp = split(src, srcWidth, delim, false);
		string westStr(westWidth, ' '), eastStr(eastWidth, ' ');
		unsigned lineWidth = westWidth + srcWidth + eastWidth;
		string northStr(lineWidth, '-'),
			southStr = northStr;

		if(westWidth) {
			northStr[0] = '.';
			westStr[0] = '|';
			southStr[0] = '\'';
		}
		if(eastWidth) {
			northStr[lineWidth-1] = '.';
			eastStr[eastWidth-1] = '|';
			southStr[lineWidth-1] = '\'';
		}

		if(north) dest << northStr << '\n';
		for(auto const& line : sp) {
			dest << westStr << line << eastStr << '\n';
			//dest << '"' << line << '"' << '\n';
		}
		if(south) dest << southStr << '\n';
		return dest;
	}*/
	/*std::ostream& border(std::ostream& dest,
			std::ostringstream const& src, unsigned srcWidth,
			unsigned westWidth = 2, unsigned eastWidth = 2,
			bool north = true, bool south = true, char delim = '\n') {
		return border(dest, src.str(), srcWidth, westWidth, eastWidth,
			north, south, delim);
	}*/
	/*
	 * @brief Columnates input; emulates GNU paste with aligned newlines
	 * @param dest The destination of the pasted columns
	 * @param lhs The contents of the left column
	 * @param lWidth The width used to wrap lines in the left column
	 * @param rhs The contents of the right column
	 * @param rWidth The width used to wrap lines in the right column
	 * @param sepWidth The width of the separation between columns
	 * @param delim The character used to identify line boundaries
	 */
	/*std::ostream& paste(std::ostream& dest,
			string const& lhs, unsigned lWidth,
			string const& rhs, unsigned rWidth,
			unsigned sepWidth = 3, char delim = '\n') {
		char div0 = '|', div1 = '|';
		auto lSplit = split(lhs, lWidth, delim, false),
			rSplit = split(rhs, rWidth, delim, false);
		unsigned lHeight = lSplit.size(), rHeight = rSplit.size(),
			maxHeight = std::max(lHeight, rHeight);
		if(!maxHeight) return dest;
		lSplit.reserve(maxHeight);
		rSplit.reserve(maxHeight);
		if(lHeight < maxHeight) {
			std::fill_n(&lSplit[lHeight], maxHeight - lHeight, "");
		}
		for(unsigned i = 0; i < maxHeight; i++) {
			string lStr, rStr;
			if(i < lHeight) lStr = lSplit[i];
			if(i < rHeight) rStr = rSplit[i];
			unsigned lSize = lStr.size(), rSize = rStr.size(),
					lLines = lSize / lWidth, rLines = rSize / rWidth,
					maxLines = std::max(lLines, rLines),
					ldLines = maxLines - lLines,
					rdLines = maxLines - rLines;
			if(lLines < maxLines)
				lStr += string(ldLines*lWidth, ' ');
			else if(rLines < lLines)
				rStr += string(rdLines*rWidth, ' ');
			for(unsigned j = 0; j < maxLines; j++) {
				dest << lStr.substr(j*lWidth, lWidth);
				if(sepWidth) {
					string sepStr(sepWidth, ' ');
					sepStr[sepWidth - 1 - int(sepWidth/2)] =
						(j ? div0 : div1);
					dest << sepStr;
				}
				dest << rStr.substr(j*rWidth, rWidth) << '\n';
			}
		}
		return dest;
	}*/
	/* std::ostream& paste(std::ostream& dest,
			std::ostringstream const& lhs, unsigned lWidth,
			std::ostringstream const& rhs, unsigned rWidth,
			unsigned sepWidth = 3, char delim = '\n') {
		return paste(dest, lhs.str(), lWidth,
			rhs.str(), rWidth, sepWidth, delim);
	}*/
	/*
	 * @brief Columnates input with aligned newlines and outer border
	 * @param dest The destination of the pasted columns
	 * @param lhs The contents of the left column
	 * @param lWidth The width used to wrap lines in the left column
	 * @param rhs The contents of the right column
	 * @param rWidth The width used to wrap lines in the right column
	 * @param sepWidth The width of the separation between columns
	 * @param bordWidth The width of the left and right outer borders
	 * @param north Enables horizontal rule prepended to output
	 * @param south Enables horizontal rule appended to output
	 * @param delim The character used to identify line boundaries
	 */
	/*std::ostream& pasteBorder(std::ostream& dest,
			string const& lhs, unsigned lWidth,
			string const& rhs, unsigned rWidth,
			unsigned sepWidth = 3, unsigned bordWidth = 1,
			bool north = true, bool south = true,
			char delim = '\n') {
		std::ostringstream oss;
		paste(oss, lhs, lWidth, rhs, rWidth, sepWidth, delim);
		if(!bordWidth) return dest << oss.str();
		string src = oss.str(), buf, border(bordWidth, ' ');
		border[0] = '|';
		std::istringstream bufRd(src);
		unsigned j = 0, jw = bordWidth*2 + sepWidth + lWidth + rWidth;
		if(north) dest << '.' << string(jw-2, '-') << '.' << '\n';
		while(getline(bufRd, buf, delim)) {
			dest << border << buf;
			std::swap(border[0], border[bordWidth-1]);
			dest << border << '\n';
			std::swap(border[0], border[bordWidth-1]);
		}
		if(south) dest << '\'' << string(jw-2, '-') << '\'' << '\n';
		return dest;
	}*/

	/*std::ostream& pasteBorder(std::ostream& dest,
			std::ostringstream const& lhs, unsigned lWidth,
			std::ostringstream const& rhs, unsigned rWidth,
			unsigned sepWidth = 3, unsigned bordWidth = 1,
			bool north = true, bool south = true,
			char delim = '\n') {
		return pasteBorder(dest, lhs.str(), lWidth,
			rhs.str(), rWidth, sepWidth, bordWidth, north, south, delim);
	}*/
}
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
