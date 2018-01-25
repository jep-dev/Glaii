/*! @file include/events.hpp
 *  @brief Events, handlers, and interface logic */

#ifndef EVENTS_HPP
#define EVENTS_HPP

///@cond
#include <SDL2/SDL_events.h>
///@endcond

#include "abstract.hpp"
#include <iostream>

namespace Abstract {

	struct FSignal {
		typedef enum Code : unsigned char {
			ok = 0,
			err = 1,
			quit = 2
		} Code;
		Code error;
		//unsigned char error : 2;
		operator bool(void) const { return error == 0; }
	};

	template<typename D>
	struct Handler_t {
		template<typename T>
		FSignal handle(T const& t) {
			return static_cast<D&>(*this).handle(t);
		}
		/* {
			return {FSignal::Code::ok};
		}*/
	private:
	};

	/** @brief Calls individual dispatchers so the handler can opt into
	 * individual event types. */
	template<typename T>
	FSignal call_handler(Handler_t<T>& hnd, SDL_Event const& ev) {
		switch(ev.type) {
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
				//return call_handler(hnd, ev.button);
				return hnd.handle(ev.button);
				//return hnd.handle(ev.button, 0);
			case SDL_MOUSEMOTION:
				return hnd.handle(ev.motion);
				//return call_handler(hnd, ev.motion);
				//return hnd.handle(ev.motion, 0);
			case SDL_WINDOWEVENT:
				return hnd.handle(ev.window);
				//return call_handler(hnd, ev.window);
				//return hnd.handle(ev.window, 0);
			case SDL_KEYDOWN:
				return hnd.handle(ev.key);
				//return call_handler(hnd, ev.key);
				//return hnd.handle(ev.key, 0);
			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_CONTROLLERBUTTONUP:
				return hnd.handle(ev.cbutton);
				//return call_handler(hnd, ev.cbutton);
				//return hnd.handle(ev.cbutton, 0);
			case SDL_QUIT:
				return hnd.handle(ev.quit);
				//return call_handler(hnd, ev.quit);
				//return hnd.handle(ev.quit, 0);
			default:
				return {FSignal::Code::ok};
		}
	}

}

#endif
