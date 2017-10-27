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

	/** @brief Reads entire ifstream into memory. */
	struct Cutter {
		template<typename T>
		using BufIterator_t = std::istreambuf_iterator<T>;

		const string data;
		operator bool(void) const;
		operator const char*(void) const;
		operator string const&(void) const;

		Cutter(void);
		Cutter(string && src);
		Cutter(string const& src);
		Cutter(const char *fname);

		template<typename T>
		Cutter(BufIterator_t<T> && p0, BufIterator_t<T> && p1 = {}):
			data(p0, p1) {}

		template<typename S>
		Cutter(S& s): data((s.good() ?
				BufIterator_t<char>(s) : BufIterator_t<char>{}), {}) {}
		template<typename S>
		Cutter(S && s): data((s.good() ?
				BufIterator_t<char>(s) : BufIterator_t<char>{}), {}) {}
			//data(std::istreambuf_iterator<char>(s), {}) {}
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
