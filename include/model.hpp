#ifndef MODEL_HPP
#define MODEL_HPP

#include "abstract.hpp"
#include "runnable.hpp"

namespace Model {

	template<typename>
	struct ModelBase;

	template<typename T>
	struct ModelBase: Abstract::Updatable<T> {
		using Abstract::Updatable<T>::update;
	};

	template<typename S, unsigned BRANCH = 0>
	struct Octree {
		typedef typename std::conditional<BRANCH,
				Octree<S, BRANCH - 1>[8], S>::type data_type;
		data_type data;
	};

}

#endif
