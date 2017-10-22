#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <map>
#include <glbinding/Binding.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

namespace View {
	using namespace gl;
	using namespace glbinding;
	struct Window;
}

#include "runnable.hpp"

namespace View {

	/** @brief RAII and operations on an SDL window/context pair. */
	struct Window: Abstract::Updatable<Window> {
	protected:
		SDL_Window *win;
		Streams::ErrorFIFO errors;
		SDL_GLContext ctx;
		bool live;
	public:
		typedef Window derived_type;
		using Abstract::Updatable<Window>::get_derived;
		operator bool(void) const;
		operator SDL_Window *const(void) const;
		operator SDL_GLContext const(void) const;

		friend std::ostream&
		operator<<(std::ostream &dest, Window const& src);
		bool validate(void);

		bool handle(SDL_WindowEvent const& ev);
		bool handle(SDL_KeyboardEvent const& ev);
		bool handle(SDL_Event const& ev);

		bool update(unsigned frame);
		bool draw(unsigned frame);

		Window(const char *title, int w, int h,
			Uint32 flags, std::map<SDL_GLattr, int> const& attribs);
	};
}

#endif
