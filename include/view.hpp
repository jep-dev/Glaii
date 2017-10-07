#ifndef VIEW_HPP
#define VIEW_HPP

#include <deque>
#include <glbinding/gl/gl.h>
#include <SDL2/SDL_video.h>

#include "abstract.hpp"
#include "streams.hpp"

namespace Streams {
	struct ErrorFIFO: std::deque<std::string> {
		operator bool(void) const {
			return size();
		}
		bool operator()(void) {
			*this << SDL_GetError();
			SDL_ClearError();
			return *this;
		}
		template<typename T>
		ErrorFIFO& operator<<(T const& t) {
			std::ostringstream oss;
			return (oss << t), (*this << oss.str());
		}
		template<typename T>
		ErrorFIFO& operator<<(T && t) {
			std::ostringstream oss;
			return (oss << t), (*this << oss.str());
		}
		ErrorFIFO& operator<<(std::string && src) {
			if(src.size()) emplace_back(src);
			return *this;
		}
		template<typename T>
		friend T& operator<<(T &dest, ErrorFIFO const& src) {
			for(auto const& i : src)
				dest << i << '\n';
			return dest;
		}
	};
}

namespace View {
	using namespace Streams;
	using namespace gl;
}

#endif
