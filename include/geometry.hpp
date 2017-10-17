#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "abstract.hpp"

namespace Geometry {

	template<typename S, unsigned BRANCH = 0>
	struct Octree {
		typedef typename std::conditional<BRANCH,
				Octree<S, BRANCH - 1>[8], S>::type data_type;
		data_type data;
	};

}

#endif
