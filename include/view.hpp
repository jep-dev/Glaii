#ifndef VIEW_HPP
#define VIEW_HPP

///@cond
#include <deque>
#include <sstream>
#include <string>

#include <glbinding/gl/gl.h>
#include <SDL2/SDL_video.h>
///@endcond

#include "abstract.hpp"

namespace Streams {
	/** @brief Deque of non-empty strings interpreted as errors. */
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
	using namespace gl;

	/**
	 * @brief Calculates the FOV based on orthogonal FOV and aspect ratio
	 * @tparam T The type of the aspect ratio and each FOV
	 * @return The FOV according to the standard calculation */
	template<typename T>
	T fov(T opposite, T aspect) {
		return 2*atan(tan(opposite/2)*aspect);
	}
	/**
	 * @brief The weighted sum of two FOV values, e.g. ideal and calculated
	 * @tparam T The type of all parameters and the return value
	 * @return The FOV obtained from easing between the two given FOV values.
	 */
	template<typename T>
	T fovEase(T src, T dest, T aspect, T ease = 0.5) {
		return fov(src, aspect)*ease + fov(dest, aspect)*(1-ease);
	}
	/**
	 * @brief The aspect ratio calculated by easing the given FOV's.
	 * @tparam T The type of all parameters and the return value.
	 */
	template<typename T>
	T aspectEase(T src, T dest, T aspect, T ease = 0.5) {
		return tan(fovEase(dest, src, aspect, ease)/2);
	}
}

#include "glsl.hpp"
#include "window.hpp"

#endif
