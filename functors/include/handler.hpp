#ifndef HANDLER_HPP
#define HANDLER_HPP

#include "compose.hpp"
//#include "events.hpp"
#include <type_traits>

template<typename... V>
struct Container {};


template<typename SU, typename OB>
struct HasHandle: std::is_void<decltype(std::declval<SU>()
		.handle(std::declval<OB>()))> {};

template<typename SU, typename OB>
using HasHandle_t = typename HasHandle<SU,OB>::type;


template<typename SU, typename OB>
struct Handler;
template<typename SU, typename OB>
struct Handler: std::enable_if_t<
	HasHandle_t<SU,OB>::value,
	Functor<SU, decltype(&SU::handle)>> {
	//using Functor<SU, &SU::handle>::operator();
};
/*template<typename OUT, typename ACTOR, typename... OBJECTS>
auto make_functor(OUT (ACTOR::*const action)(OBJECTS...),
	ACTOR *const actor = 0)
-> Functor<ACTOR, OUT (ACTOR::*)(OBJECTS...)>
	{ return {actor, action}; }*/

template<typename OB, typename SU>
auto make_handler(SU *const su)
		-> decltype(make_functor(&SU::handle, su))
	{ return make_functor(&SU::handle, su); }




/*template<> struct EventHandler<Event> {
	template<typename S>
	std::string handle(S && ev) const
		{ return "<<unknown>>"; }
	std::string handle(Event && ev) const {
		switch(ev.type) {
			case QuitEventType:     return handle(ev.evQuit);
			case KeyboardEventType: return handle(ev.evKeyboard);
			case MouseEventType:    return handle(ev.evMouse);
			default:                return "<<unknown type>>";
		}
	}
};

template<typename... T> struct EventHandlers;
template<typename S, typename... T>
struct EventHandlers<S,T...>:
EventHandler<S>, EventHandlers<T...> {
	using EventHandler<S>::handle;
	using EventHandlers<T...>::handle;
};
template<typename... T>
struct EventHandlers:
EventHandler<CommonEvent> {
	using EventHandler<CommonEvent>::handle;
};
*/

#endif
