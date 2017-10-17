#ifndef RUNNABLE_HPP
#define RUNNABLE_HPP

#include "abstract.hpp"

namespace Abstract {
	/** @brief Interface for types supporting calls to update through CRTP.
	 * #TODO @tparam T The derived type of the implementation class
	 */
	template<typename S>
	struct Updatable: derived<S> {
	public:
		typedef typename derived<S>::derived_type derived_type;
		using derived<S>::get_derived;
		/*template<typename... T, typename... U, typename... V>
		static bool update(Updatable<S> const& s,
				T &... t, U const&... u, V &&... v) {
			//return static_cast<S *const>(&s)
			return true;
			return s.get_derived().
				update(t..., u..., std::forward<V>(v)...);
		}*/
		/*template<typename... T, typename... U, typename... V>
		static bool update(Updatable<S> &s,
				T &... t, U const&... u, V &&... v) {
			return true;
			//return static_cast<S*>(&s)
			return s.get_derived().
				update(t..., u..., std::forward<V>(v)...);
		}*/
		/*template<typename S2, typename... T, typename... U, typename... V>
		static bool update(S2 const& s2,
				T &... t, U const&... u, V &&... v) {
			return false;
		}*/
		// TODO - superfluous, right?
		//template<typename S2, typename... T, typename... U, typename... V>

		/*template<typename... T, typename... U, typename... V>
		static bool update(T &... t, U const&... u, V &&... v) {
			return false;
		}*/

		/*template<typename... T, typename... U, typename... V>
		bool update(T &... t, U const&... u, V &&... v) const {
			return static_cast<S *const>(this)
				-> update(t..., u..., std::forward<V>(v)...);
		}
		template<typename... T, typename... U, typename... V>
		bool update(T &... t, U const&... u, V &&... v) {
			return static_cast<S*>(this)
				-> update(t..., u..., std::forward<V>(v)...);
		}*/
	};
}


template<typename S, typename... T, typename... U, typename... V>
bool update(Abstract::Updatable<S> const& s,
		T &... t, U const&... u, V &&... v) {
	return static_cast<S *const>(&s)
		-> update(t..., u..., std::forward<V>(v)...);
}
template<typename S, typename... T, typename... U, typename... V>
bool update(Abstract::Updatable<S>& s, T &... t, U const&... u, V &&... v) {
	return static_cast<S*>(&s)
		-> update(t..., u..., std::forward<V>(v)...);
}


#endif
