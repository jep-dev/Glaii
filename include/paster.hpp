#ifndef PASTER_HPP
#define PASTER_HPP

namespace Streams {
	template<typename... T> ostringstream
	repeat(ostringstream && dest, T const&... tail)
		{ return std::move(dest); }
	template<typename H, typename HN, typename... T> ostringstream
	repeat(H const& head, HN const& reps, T const&... tail)
		{ return repeat(ostringstream(), head, reps, tail...); }
	template<typename H, typename HN, typename... T> ostringstream
	repeat(ostringstream && dest,
			H const& head, HN const& reps, T const&... tail) {
		for(auto i = 0; i < reps; i++) dest << head << '\n';
		return repeat(std::move(dest), tail...);
	}
	// No newline?
	template<typename H, typename... T> ostringstream
	repeat(ostringstream && dest, H const& head, T const&... tail) {
		dest << head;
		return repeat(std::move(dest), tail...);
	}

	template<typename... T> ostringstream
	column(ostringstream && oss, T const&... tail)
		{ return std::move(oss); }
	template<typename... T> ostringstream
	column(T const&... tail) {
		return column(std::move(ostringstream()), tail...);
		/*ostringstream oss;
		return column(std::move(oss), tn...);*/
	}
	template<typename H, typename... T> ostringstream
	column(ostringstream && oss, H const& head, T const&... tail) {
		oss << head << '\n';
		return column(std::move(oss), tail...);
	}

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
		bool flushed(void) const {
			unsigned len = 0, cur = 0;
			bool once = true;
			for(auto const& line : data)
				if(len < (cur = line.size())) {
					if(!once) return false;
					else len = cur;
				}
			return true;
		}
		// TODO DRY; replace flushed, widest with pair{least,most}?
		// Computationally, flushed <= widest <= (non-const) flush
		unsigned widest(void) const {
			unsigned len = 0, cur = 0;
			for(auto const& line : data) {
				if(len < (cur = line.size()))
					len = cur;
			}
			return len;
		}
		std::size_t size(void) const {
			return data.size();
		}
		string operator[](unsigned i) const {
			return i < size() ? data[i] : "";
		}
		Paster& flush(size_t toCol = 0, size_t toRow = 0);
		Paster& operator<<(string const& rhs);
		Paster& operator<<(ostringstream const& rhs);
		// TODO DRY!
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
		Paster& repeat(ostringstream& dest);
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
		Paster(Paster const& p): data(p.data) {}
		template<typename... T>
		Paster(T const&... t): Paster() {
			*this << string(t...);
		}
	};

	template<typename S>
	S& border(S &dest, Paster const& src,
			bool n = true, bool e = true,
			bool s = true, bool w = true) {
		Paster p = src;
		ostringstream padder;
		if(!p.flushed()) p.flush();
		unsigned width = p[0].size(), height = p.size();
		auto filler = string(width, '-');
		if(n) {
			if(w) dest << ".-";
			dest << filler;
			if(e) dest << "-.";
			dest << '\n';
		}
		for(unsigned i = 0; i < height; i++) {
			if(w) dest << "| ";
			dest << p[i];
			if(e) dest << " |";
			dest << '\n';
		}
		if(s) {
			if(w) dest << "'-";
			dest << filler;
			if(e) dest << "-'";
			dest << '\n';
		}
		return dest;
	}
}

#endif
