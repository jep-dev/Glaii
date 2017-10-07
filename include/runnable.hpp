#ifndef RUNNABLE_HPP
#define RUNNABLE_HPP

#include "abstract.hpp"

namespace Abstract {
	/** @brief Interface for types supporting calls to update through CRTP.
	 * @tparam T The derived type of the implementation class
	 */
	template<typename T>
	struct Updatable: derived<T> {
		int update(void) {
			return static_cast<T*>(this) -> update();
		}
		int update(void) const {
			return static_cast<T *const>(this) -> update();
		}
	};
}

#endif
