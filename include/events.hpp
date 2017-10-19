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
		bool handle(R const& r, int) {
			return get_derived().handle(r);
		}
	public:
		template<typename R>
		bool handle(R && r) const {
			return handle(r, 0);
		}
		template<typename R>
		bool handle(R const& r) const {
			return handle(r, 0);
		}
	};

}

#endif
