#ifndef CUTTER_HPP
#define CUTTER_HPP

namespace Streams {
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
}

#endif
