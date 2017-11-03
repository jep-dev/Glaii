#include "abstract.hpp"
#include <iostream>

using namespace Abstract;

template<template<typename...> class> struct Tag_tt;
template<template<typename...> class TT> struct Tag_tt {
	using type = Tag_tt<TT>;
};
template<typename...> struct Tagged_tt {};

template<typename... TN> struct Tags_t;
template<typename... TN> struct Tags_t {
	using type = Tags_t<TN...>;
	template<typename... SN>
	constexpr auto operator<<(Tags_t<SN...> const&) const
		-> Tags_t<TN...,SN...> { return {}; }

	template<typename... SN>
	constexpr auto operator>>(Tags_t<SN...> const&) const
		-> Tags_t<SN...,TN...> { return {}; }

	template<typename S>
	constexpr auto operator+(S const&) const {
		return Tags_t<type, S>{};
	}
};
template<template<typename...> class TT, typename... TN>
auto operator*(Tag_tt<TT> const&, Tags_t<TN...> const&) {
	return Tags_t<TT<TN>...>{};
}

template<typename... SN, typename... TN>
constexpr auto tagAppend(Tags_t<SN...> const&,
		Tags_t<TN...> const&) {
	return Tags_t<SN...,TN...>{};
}

template<typename... TN>
void apply(Tags_t<TN...> = {}) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

int main(int argc, const char *argv[]) {
	constexpr auto t0 = Tags_t<int> {};
	constexpr auto t1 = Tags_t<char> {};
	constexpr auto tt0 = Tag_tt<Tagged_tt> {};
	apply(t0);
	apply(t1);
	apply(t0 >> t1);
	apply(tagAppend(t0, t1));
	apply<Tags_t<Tag_tt<Tagged_tt>>>();
	apply(t0 + t1);
	apply(t0 + std::true_type {});
	apply(Tag_tt<Tagged_tt>{}*(t0<<t1));
}
