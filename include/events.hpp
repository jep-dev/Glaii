#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "abstract.hpp"
#include <SDL2/SDL_events.h>

namespace Abstract {
	template<typename D>
	struct Handler_t;
	template<typename D>
	struct Handler_t {};

	template<typename D, typename EV>
	bool call_handler(Handler_t<D>& hnd, EV const& ev, long) {
		return true;
	}
	template<typename D, typename EV>
	auto call_handler(Handler_t<D>& hnd, EV const& ev, int)
	-> decltype(static_cast<D&>(hnd).handle(ev)) {
		return static_cast<D&>(hnd).handle(ev);
	}
	template<typename D>
	bool call_handler(Handler_t<D>& hnd, SDL_Event const& ev) {
		switch(ev.type) {
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
				return call_handler(hnd, ev.button, 0);
			case SDL_MOUSEMOTION:
				return call_handler(hnd, ev.motion, 0);
			case SDL_WINDOWEVENT:
				return call_handler(hnd, ev.window, 0);
			case SDL_KEYDOWN:
				return call_handler(hnd, ev.key, 0);
			case SDL_QUIT:
				return call_handler(hnd, ev.quit, 0);
			default:
				return true;
		}
	}

}

#endif
