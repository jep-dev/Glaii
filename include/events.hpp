/*! @file include/events.hpp
 *  @brief Events, handlers, and interface logic */

#ifndef EVENTS_HPP
#define EVENTS_HPP

///@cond
#include <SDL2/SDL_events.h>
///@endcond

namespace Abstract {

	struct FSignal {
		typedef enum Code : unsigned char {
			ok = 0,
			err = 1,
			quit = 2
		} Code;
		Code error;
		const char *message = "(default)";
		operator bool(void) const { return error == ok; }
		bool operator<(Code const& c) { return error < c; }
		bool operator<(FSignal const& s) { return error < s.error; }
	};

	template<typename D>
	struct Handler_t {
		template<typename T>
		FSignal handle(T const& t) {
			return static_cast<D&>(*this).handle(t);
		}
	};

	/** @brief Calls individual dispatchers. */
	template<typename T>
	FSignal call_handler(Handler_t<T>& hnd, SDL_Event const& ev) {
		switch(ev.type) {
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
				return hnd.handle(ev.button);
			case SDL_MOUSEMOTION:
				return hnd.handle(ev.motion);
			case SDL_WINDOWEVENT:
				return hnd.handle(ev.window);
			case SDL_KEYDOWN:
				return hnd.handle(ev.key);
			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_CONTROLLERBUTTONUP:
				return hnd.handle(ev.cbutton);
			case SDL_QUIT:
				return hnd.handle(ev.quit);
			default:
				return {FSignal::Code::ok};
		}
	}

}

#endif
