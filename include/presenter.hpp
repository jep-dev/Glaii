#ifndef PRESENTER_HPP
#define PRESENTER_HPP

#include "runnable.hpp"

namespace Presenter {
	template<typename T>
	struct Presenter_t;
	/**
	 * @brief Presenter type as per model-view-presenter (future Runnable)
	 * @tparam T The derived class providing the update method
	 */
	template<typename T>
	struct Presenter_t: Abstract::Updatable<T> {
		using Abstract::Updatable<T>::update;
	};
}

#endif
