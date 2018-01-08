/*! @file include/events.hpp
 *  @brief Events, handlers, and interface logic */

#ifndef EVENTS_HPP
#define EVENTS_HPP

///@cond
#include <SDL2/SDL_events.h>
///@endcond

#include "abstract.hpp"

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

	/** @brief CRTP subscription of type D to each event type. */
	template<typename D, typename... E>
	struct Handler_t {
		template<typename E1>
		FSignal handle(E1 const& ev);
	};
	template<typename D, typename E0, typename... EN>
	struct Handler_t<D, E0, EN...>: Handler_t<D, EN...> {
		FSignal handle(E0 const& ev) {
			return static_cast<D&>(*this).handle(ev);
		}
		using Handler_t<D, EN...>::handle;
	};

	template<typename D, typename... EN, typename E0>
	FSignal call_handler(Handler_t<D, EN...>& hnd, E0 const& ev) {
		return {0};
	}
	template<typename D, typename E0, typename... EN>
	FSignal call_handler(Handler_t<D, E0, EN...>& hnd, E0 const& ev) {
		return hnd.handle(ev);
	}

	/** @brief Calls individual dispatchers so the handler can opt into
	 * individual event types. */
	template<typename D, typename... EN>
	FSignal call_handler(
		Handler_t<D, EN...>& hnd, SDL_Event const& ev) {
		//D& hnd, SDL_Event const& ev) {
		switch(ev.type) {
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
				return call_handler(hnd, ev.button);
			case SDL_MOUSEMOTION:
				return call_handler(hnd, ev.motion);
			case SDL_WINDOWEVENT:
				return call_handler(hnd, ev.window);
			case SDL_KEYDOWN:
				return call_handler(hnd, ev.key);
			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_CONTROLLERBUTTONUP:
				return call_handler(hnd, ev.cbutton);
			case SDL_QUIT:
				return call_handler(hnd, ev.quit);
			default:
				return {0};
		}
	}

}

#endif
