#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "view.hpp"
// #include "glsl.hpp"
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

	/** @brief RAII and operations for SDL-authored window/context pair. */
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
		/*Window(const char *title, int w, int h,
			Uint32 flags, std::map<SDL_GLattr, int> const& attribs) {
			do {
				if(errors()) {
					errors.emplace_back("Window creation disabled.");
					break;
				}
				int i = 0;
				//for(auto const& attr : {attribs...})
				for(auto const& attr : attribs)
					SDL_GL_SetAttribute(attr.first, attr.second);
				win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED, w, h, flags | SDL_WINDOW_OPENGL);
				if(errors() || !win) {
					errors.push_back("SDL window could not be created.");
					break;
				}
				ctx = SDL_GL_CreateContext(win);
				if(!ctx) {
					errors.push_back("SDL context could not be created.");
					break;
				}
				SDL_GL_MakeCurrent(win, ctx);
				Binding::initialize(false);
				for(auto const& attr : attribs) {
					i++;
					SDL_GLattr k = attr.first;
					// SDL_GLattr const& k = std::get<0>(attr);
					int v0 = attr.second, v1 = v0;
					//int v0 = std::get<1>(attr), v1 = v0;
					SDL_GL_GetAttribute(k, &v1);
					if(errors() || (v0 != v1)) {
						std::ostringstream oss;
						oss << "Attribute #" << i << ": ["
							<< k << "] = " << v1 << " != " << v0;
						errors.push_back(oss.str());
						break;
					}
				}
				live = true;
				SDL_GL_SetSwapInterval(1);
				return;
			} while(0);
			live = false;
		}*/
	};
}

#endif
