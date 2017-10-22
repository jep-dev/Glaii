#ifndef ABSTRACT_HPP
#define ABSTRACT_HPP

#include <functional>
#include <type_traits>
#include <ostream>

// Deduce type from expression, generally for template arg defaults
// TODO more generalized deductions than just infix operators?
#ifndef COMBINE
#define COMBINE(X,OP,Y) decltype(std::declval<X>() OP std::declval<Y>())
#endif

namespace Abstract {

	/*-- CRTP helper types --*/
	/** @brief Helper type analogous to null-terminator for strings. */
	struct null_type {};
	template<typename> struct enable;
	template<typename> struct derived;
	template<typename T> struct tag_id;
	/**
	 * @brief General-purpose type label/tag.
	 * @tparam T The tagged type (optional)
	 */
	template<typename T = null_type> struct tag_type {};
	/**
	 * @brief Alias for integral constant to match tag_ pattern
	 * @tparam R The type of the value
	 * @tparam T The value itself
	 */
	template<typename R, R T>
	using tag_value = std::integral_constant<R,T>;

	/** @brief Interface contract used in combination with enable type. */
	template<typename S, typename... T> struct intf_require;

	/*-- Value reductions --*/
	/** @brief Variadic template OR type (base case). */
	template<bool...>
	struct any_true: std::false_type {};

	/** @brief Variadic template OR type (recursive case). */
	template<bool B1, bool... BN>
	struct any_true<B1, BN...>: std::conditional<B1,
		std::true_type, any_true<BN...>> {};

	/** @brief Helper type used to induce substitution failure in SFINAE. */
	template<typename>
	struct enable: std::true_type {};

	template<typename S, typename... T>
	auto implements(long) -> std::false_type;

	template<typename S, typename... T>
	auto implements(int)
		-> enable<decltype(S::apply(std::declval<T>()...))>;

	/**
	 * @brief Helper type using CRTP for compile-time polymorphism.
	 * @tparam D The type identified final result of derivation
	 */
	template<typename D>
	struct derived {
		typedef D derived_type;
		D const& get_derived(void) const {
			return *static_cast<D *const>(this);
		}
		D& get_derived(void) {
			return *static_cast<D*>(this);
		}
	};
	/**
	 * @brief Base class for types promising implementation.
	 * @tparam S The type promising or providing the implementation
	 * @tparam T The types of arguments into the promised function
	 */
	template<typename S, typename... T>
	struct intf_require: decltype(implements<S,T...>(0)) {};

	/*
	 * @brief Creates a new sequence of UIDs for each template argument
	 * @tparam D The tagged type, not necessarily derived from the tag itself
	 */
	template<typename D>
	struct tag_id {
	private:
		static unsigned next_id(bool inc) {
			static unsigned last_id = 0;
			if(inc) last_id++;
			return last_id;
		}
		const unsigned id = next_id(true);
	public:
		static unsigned next_id(void) {
			return next_id(false);
		}
		unsigned get_id(void) const { return id; }
	};

	/**
	 * @brief Inserts the instance's UID out of total issued of its type
	 * @tparam D The tagged type, key to a UID sequence
	 * @param dest The destination stream to insert the tagged instance
	 * @param d The tag instance (not necessarily an instance of D)
	 * @return The destination stream after insertion of the tag
	 */
	template<typename D>
	std::ostream& operator<<(std::ostream& dest, tag_id<D> const& d) {
		return dest << '#' << d.get_id() << '/' << tag_id<D>::next_id();
	}
}

#endif
