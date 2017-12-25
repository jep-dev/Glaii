/*! @file include/events.hpp
 *  @brief Events, handlers, and interface logic */

#ifndef EVENTS_HPP
#define EVENTS_HPP

///@cond
#include <SDL2/SDL_events.h>
///@endcond

#include "abstract.hpp"

namespace Abstract {
	/**
	 * @brief Identifies D as a base with the desired handler implementation.
	 * @tparam D The targeted type; passes unhandled events implicitly.
	 */
	template<typename D>
	struct Handler_t {};

	/**
	 * @brief Policy for handlers unprepared for the given event.
	 * @tparam D The type expected to handle the event
	 * @tparam EV The type of the event to handle
	 * @param hnd The handler: not type D or missing handle(EV)
	 * @param ev The event to handle
	 * @return True, vacuously - handle(EV) must be defined to fail.
	 */
	template<typename D, typename EV>
	bool call_handler(Handler_t<D>& hnd, EV const& ev, long) {
		return true;
	}

	/**
	 * @brief Passes an event to the (projected) handler.
	 * @tparam D The type known and designated to handle the event
	 * @tparam EV The type of the event to handle
	 * @param hnd The handler instance intended to receive the event
	 * @param ev The event to handle
	 * @return The type returned by the projected type's
	 * corresponding method.
	 */
	template<typename D, typename EV>
	auto call_handler(Handler_t<D>& hnd, EV const& ev, int)
	-> decltype(static_cast<D&>(hnd).handle(ev)) {
		return static_cast<D&>(hnd).handle(ev);
	}
	/**
	 * @brief Separates event components since this is almost certainly
	 * not the responsibility of the handler.
	 * @tparam D The handler's implementation type
	 * @param hnd The instance given to handle event components
	 * @param ev The event as its base, undifferentiated 'events'
	 */
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
