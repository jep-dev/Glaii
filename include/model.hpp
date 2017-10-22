#ifndef MODEL_HPP
#define MODEL_HPP

#include "abstract.hpp"
#include "runnable.hpp"

namespace Model {

	template<typename>
	struct ModelBase;

	/**
	 * @brief Abstract Updatable model type (future Runnable)
	 * @tparam T The derived class providing the update method
	 */
	template<typename T>
	struct ModelBase: Abstract::Updatable<T> {
		using Abstract::Updatable<T>::update;
	};

	/*template<typename S, unsigned BRANCH = 0>
	struct Octree { // TODO
		typedef typename std::conditional<BRANCH,
				Octree<S, BRANCH - 1>[8], S>::type data_type;
		data_type data;
	};*/

}

#endif
