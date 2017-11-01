#ifndef PASTER_HPP
#define PASTER_HPP

namespace Streams {
	template<typename... TN>
	ostringstream column(ostringstream && oss, TN const&... tn) {
		return std::move(oss);
	}
	template<typename T0, typename... TN>
	ostringstream column(ostringstream && oss,
			T0 const& t0, TN const&... tn) {
		oss << t0 << '\n';
		return column(std::move(oss), tn...);
	}
	template<typename... TN>
	ostringstream column(TN const&... tn) {
		ostringstream oss;
		return column(std::move(oss), tn...);
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
		Paster& flush(size_t toCol = 0, size_t toRow = 0);
		Paster& operator<<(string const& rhs);
		Paster& operator<<(ostringstream const& rhs);
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
		template<typename... T>
		Paster(T const&... t): Paster() {
			*this << string(t...);
		}
	};
}

#endif