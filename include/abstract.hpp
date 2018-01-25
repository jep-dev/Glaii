/*! @file abstract.hpp
 *  @brief TMP utilities, e.g. instance counters, CRTP bases, etc. */

#ifndef ABSTRACT_HPP
#define ABSTRACT_HPP

///@cond
#include <functional>
#include <type_traits>
///@endcond

// Deduce type from expression, generally for template arg defaults
// TODO more generalized deductions than just infix operators?
#ifndef COMBINE
#define COMBINE(X,OP,Y) decltype(std::declval<X>() OP std::declval<Y>())
#endif

namespace Abstract {

	/** @brief Helper type used in SFINAE reasoning about missing types. */
	template<typename...> struct Void { typedef void type; };

	/** @brief Type member alias following C++ convention. */
	template<typename... T> using Void_t = typename Void<T...>::type;

	/** @brief Helper type analogous to null-terminator for strings. */
	struct Null_type {};

	/** @brief Provides a per-type UID to count and track copies/instances.
 	 * @tparam T Instance type corresponding to a unique ID sequence */
	template<typename T> struct Tag_id;

	/** @brief General-purpose type label/tag.
	 *  @tparam T The tagged type (optional) */
	template<typename T = Null_type> struct Tag_type {};
	/** @brief Alias for integral constant to match tag_ pattern
	 *  @tparam R The type of the value
	 *  @tparam T The value itself
	 */
	template<typename R, R T>
	using Tag_value = std::integral_constant<R,T>;

	/*-- Value reductions --*/

	/** @brief Variadic template OR type (base case). */
	template<bool...>
	struct Any_true: std::false_type {};

	/** @brief Variadic template OR type (recursive case). */
	template<bool B1, bool... BN>
	struct Any_true<B1, BN...>:
		std::conditional<B1, std::true_type, Any_true<BN...>> {};

	/** @brief Creates a new sequence of UIDs for each template argument
	 *  @tparam D The tagged type, not necessarily derived from the tag */
	template<typename D>
	struct Tag_id;

}

#endif
