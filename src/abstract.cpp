#include "abstract.hpp"
#include <iostream>

namespace Abstract {

	template<typename S>
	unsigned get_id(S const& s) {
		return S::get_id(s);
	}

	template<typename S>
	std::ostream& operator<<(std::ostream &os, tag_id<S> const& t) {
		return os << "#" << get_id(t);
	}

	template<typename S>
	std::ostream& operator<<(std::ostream &os, derived<S> const& t) {
		return os << t.get_derived();
	}

}
