#ifndef COMPOSE_HPP
#define COMPOSE_HPP

#include <algorithm>
#include <iostream>
#include <ostream>
#include <string>
#include <sstream>
#include <utility>

template<typename ACTOR, typename ACTION>
struct Functor {
	typedef ACTOR actor_type;
	typedef const ACTION action_type;

	actor_type *const actor;
	action_type action;

	template<typename... I> auto operator()(I &&... args)
		{ return ((*actor).*action)(std::forward<I>(args)...); }
	template<typename... I> auto operator()(I &&... args) const
		{ return ((*actor).*action)(std::forward<I>(args)...); }
};

template<typename OUT, typename ACTOR, typename... OBJECTS>
auto make_functor(OUT (ACTOR::*const action)(OBJECTS...),
	ACTOR *const actor = 0)
-> Functor<ACTOR, OUT (ACTOR::*)(OBJECTS...)>
	{ return {actor, action}; }

template<typename OUT, typename ACTOR, typename... OBJECTS>
auto make_functor(OUT (ACTOR::*const action)(OBJECTS...) const,
	ACTOR *const actor = 0)
-> Functor<ACTOR, OUT (ACTOR::*const )(OBJECTS...) const>
	{ return {actor, action}; }

#endif
