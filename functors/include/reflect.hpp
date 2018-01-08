#ifndef REFLECT_HPP
#define REFLECT_HPP

/*template<typename... T>
struct CRTP;

template<typename S, typename... T>
struct CRTP: private CRTP<T...> {
	template<typename R>
	friend auto derive(R && r, long)
		-> decltype(CRTP<T...>::derive(r))
			{ return CRTP<T...>::derive(r); }
	friend auto derive(S && s, int)
		{ static_cast<S>(derive(r)); }

};
template<typename S, typename... T>
struct CRTP<


	template<typename S>
	friend S && derive(S && s)
		{ return std::forward<S>(s); }

	template<typename... S>
	CRTP<T...>(S &&... s): trace({s}...) {}
	std::vector<std::string> trace;
};

template<typename HEAD, typename... TAIL>
struct CRTP<HEAD,TAIL...> {
	typedef HEAD derived;
	typedef CRTP<TAIL...> delegate;
	typedef CRTP<HEAD,TAIL...> self;

	friend derived const& derive(CRTP<derived, TAIL...> const& c)
		{ return static_cast<derived const&>(c); }
	friend derived & derive(CRTP<derived, TAIL...> & c)
		{ return static_cast<derived &>(c); }

	template<typename... T>
	CRTP(T &&... t): delegate(__PRETTY_FUNCTION__,
		std::forward<T>(t)...) {}
};
/*
template<typename... T>
struct CRTP {
	template<typename S>
	friend S && derive(S && s)
		{ return std::forward<S>(s); }

	template<typename... S>
	CRTP<T...>(S &&... s): trace({s}...) {}
	std::vector<std::string> trace;
};

template<typename HEAD, typename... TAIL>
struct CRTP<HEAD,TAIL...> {
	typedef HEAD derived;
	typedef CRTP<TAIL...> delegate;
	typedef CRTP<HEAD,TAIL...> self;

	friend derived const& derive(CRTP<derived, TAIL...> const& c)
		{ return static_cast<derived const&>(c); }
	friend derived & derive(CRTP<derived, TAIL...> & c)
		{ return static_cast<derived &>(c); }

	template<typename... T>
	CRTP(T &&... t): delegate(__PRETTY_FUNCTION__,
		std::forward<T>(t)...) {}
};
*/


#endif
