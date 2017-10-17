#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "abstract.hpp"

namespace Abstract {
	template<typename T>
	struct AbstractHandler;

	template<typename T>
	struct AbstractHandler: derived<T> {
	private:
		template<typename R>
		bool handle(R && r, int) {
			return get_derived().handle(r);
		}
		template<typename R>
		bool handle(R && r, long) {
			return true;
		}
		template<typename R>
		bool handle(R && r, int) const {
			return get_derived().handle(r);
		}
		template<typename R>
		bool handle(R && r, long) const {
			return true;
		}
	public:
	};

}

#endif
