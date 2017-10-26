#ifndef RUNNABLE_HPP
#define RUNNABLE_HPP

#include "abstract.hpp"

namespace Abstract {
	/** @brief Interface for types supporting calls to update through CRTP.
	 * @tparam D The derived type of the implementation class
	 */
	template<typename D>
	struct Updatable_t {};
}

/**
 * @brief Updates the given constant subject using its derived implementation
 * @tparam S The class providing the derived implementation of update
 * @tparam T The types of the referenced arguments
 * @tparam U The types of the referenced constant arguments
 * @tparam V The types of the arguments given by value
 * @param s The subject which should be updated
 * @param t The referenced arguments
 * @param u the referenced constant arguments
 * @param v The arguments given by value
 * @return True if and only if the derived implementation returns true
 */
template<typename S, typename... T, typename... U, typename... V>
bool update(Abstract::Updatable_t<S> const& s,
		T &... t, U const&... u, V &&... v) {
	return static_cast<S const&>(s)
		.update(t..., u..., std::forward<V>(v)...);
}
/**
 * @brief Updates the given subject using its derived implementation
 * @tparam S The class providing the derived implementation of update
 * @tparam T The types of the referenced arguments
 * @tparam U The types of the referenced constant arguments
 * @tparam V The types of the arguments given by value
 * @param s The subject which should be updated
 * @param t The referenced arguments
 * @param u the referenced constant arguments
 * @param v The arguments given by value
 * @return True if and only if the derived implementation returns true
 */
template<typename S, typename... T, typename... U, typename... V>
bool update(Abstract::Updatable_t<S>& s,
		T &... t, U const&... u, V &&... v) {
	return static_cast<S&>(s)
		.update(t..., u..., std::forward<V>(v)...);
}


#endif
