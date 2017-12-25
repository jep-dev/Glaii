/*! @file include/model.hpp
 *  @brief Stub for models; closely linked to 'geometry'. */

#ifndef MODEL_HPP
#define MODEL_HPP

#include "abstract.hpp"
#include "runnable.hpp"

namespace Model {

	template<typename>
	struct ModelBase;

	/** @brief Abstract Updatable model type (future Runnable)
	 *  @tparam T The derived class providing the update method
	 */
	template<typename T>
	struct ModelBase: Abstract::Updatable_t<T> {
		using Abstract::Updatable_t<T>::update;
	};

}

#endif
