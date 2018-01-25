#ifndef ABSTRACT_TPP
#define ABSTRACT_TPP

/** @file include/abstract.tpp
 *  @brief Implementations from declarations in abstract.hpp */

#include "abstract.hpp"

namespace Abstract {
	template<typename D>
	struct Tag_id {
	private:
		static unsigned next_id(bool inc) {
			static unsigned last_id = 0;
			if(inc) last_id++;
			return last_id;
		}
		const unsigned id = next_id(true);
	public:
		static unsigned next_id(void) {
			return next_id(false);
		}
		unsigned get_id(void) const { return id; }
	};
}
#endif
