/*! @file include/presenter.hpp
 *  @brief Contains the stub of the interface between model and view */

#ifndef PRESENTER_HPP
#define PRESENTER_HPP

#include "runnable.hpp"

namespace Presenter {
	template<typename T>
	struct Presenter_t;
	/** @brief Presenter type as per model-view-presenter (future Runnable)
	 *  @tparam T The derived class providing the update method
	 */
	template<typename T>
	struct Presenter_t: Abstract::Updatable_t<T> {
		using Abstract::Updatable_t<T>::update;
	};
}

#endif
