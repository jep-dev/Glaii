#ifndef ABSTRACT_HPP
#define ABSTRACT_HPP

#include <functional>
// #include <algorithm>
#include <iosfwd>

namespace Abstract {

	/*-- CRTP helper types --*/
	/** @brief Helper type analogous to null-terminator for strings. */
	struct null_type {};
	/** @brief Helper type used to induce substitution failure in SFINAE. */
	template<typename> struct enable;
	/** @brief Helper type using CRTP for compile-time polymorphism. */
	template<typename> struct derived;
	/** @brief Helper type giving a per-type UID to instances. */
	template<typename T, typename = T> struct tag_id;
	/** @brief General-purpose tag encapsulator. */
	template<typename = null_type> struct tag_type {};
	/** @brief Interface contract used in combination with enable type. */
	template<typename S, typename... T> struct intf_require;

	/*-- Value reductions --*/
	/** @brief Variadic template OR type (base case). */
	template<bool...>
	struct any_true: std::false_type {};

	/** @brief Variadic template OR type (induction). */
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
