#ifndef ABSTRACT_HPP
#define ABSTRACT_HPP

#include <functional>
// #include <algorithm>
#include <iosfwd>

namespace Abstract {

	/*-- CRTP helper types --*/
	struct null_type {};
	template<typename> struct enable;
	template<typename> struct derived;
	template<typename T, typename = T> struct tag_id;
	template<typename = null_type> struct tag_type {};
	template<typename S, typename... T> struct intf_require;

	/*-- Memory management --*/
	template<typename T> struct managed;

	/*-- Value reductions --*/
	template<bool...>
	struct any_true: std::false_type {};

	template<bool B1, bool... BN>
	struct any_true<B1, BN...>: std::conditional<B1,
		std::true_type, any_true<BN...>> {};

	template<typename>
	struct enable: std::true_type {};

	template<typename S, typename... T>
	auto implements(long) -> std::false_type;

	template<typename S, typename... T>
	auto implements(int)
		-> enable<decltype(S::apply(std::declval<T>()...))>;

	template<typename D>
	struct derived {
		typedef D derived_type;
		D& get_derived(void) {
			return *static_cast<D*>(this);
		}
	};
	template<typename S, typename... T>
	struct intf_require: decltype(implements<S,T...>(0)) {};

	template<typename T, typename D>
	struct tag_id: derived<D> {
	private:
		static unsigned next_id(void) {
			static unsigned last_id = 0;
			return last_id++;
		}
		const unsigned id = next_id();
	public:
		unsigned get_id(void) const { return id; }
		static unsigned get_id(tag_id<T> const& t) {
			return t.get_id();
		}
	};
	/*template<typename T>
	struct managed: tag_id<managed<T>> {
		T data;
		bool manage;
		void (*dtor) (T);
		operator T(void) const { return data; }
		managed(T t, bool manage, void (*fn)(T) = nullptr):
			data(t), manage(data && manage && dtor), dtor(fn) {}
		virtual ~managed(void) { if(manage) dtor(data); }
	};*/
	template<typename S>
	unsigned get_id(S const& s) {
		return S::get_id(s);
	}
	template<typename S> std::ostream&
		operator<<(std::ostream &os, tag_id<S> const& t);
	template<typename S> std::ostream&
		operator<<(std::ostream &os, derived<S> const& t);
}

#endif
