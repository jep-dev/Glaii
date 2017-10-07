#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "view.hpp"
#include "glsl.hpp"
#include "runnable.hpp"
#include <glbinding/Binding.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

namespace View {
	using namespace gl;
	using namespace glbinding;

	using Attrib = std::tuple<const SDL_GLattr, const int, int>;
	Attrib attrib(SDL_GLattr k, int v) {
		return std::make_tuple(k, v, v);
	}

	/** @brief RAII and operations for SDL-authored window/context pair. */
	struct Window: Abstract::Updatable<Window> {
	protected:
		SDL_Window *win;
		Streams::ErrorFIFO errors;
		SDL_GLContext ctx;
		bool live;
	public:
		operator bool(void) const;
		operator SDL_Window *const(void) const;
		operator SDL_GLContext const(void) const;

		friend std::ostream&
		operator<<(std::ostream &dest, Window const& src);
		template<GLenum E0, GLenum... EN>
		Window& operator<<(Shaders::Program<E0,EN...> const& p);
		bool validate(void);
		bool handle(SDL_WindowEvent const& ev);
		bool handle(SDL_KeyboardEvent const& ev);
		bool handle(SDL_Event const& ev);
		bool update(void);
		bool draw(void);
		template<typename... S>
		Window(const char *title, int w, int h,
				Uint32 flags, S &&... attribs) {
			do {
				if(errors()) {
					errors.push_back("Errors precede Window creation.");
					break;
				}
				win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED, w, h, flags | SDL_WINDOW_OPENGL);
				if(errors() || !win) {
					errors.push_back("SDL window could not be created.");
					break;
				}
				int i = 0;
				for(auto attr : {attribs...})
					SDL_GL_SetAttribute(std::get<0>(attr), std::get<2>(attr));
				ctx = SDL_GL_CreateContext(win);
				if(!ctx) {
					errors.push_back("SDL context could not be created.");
					break;
				}
				SDL_GL_MakeCurrent(win, ctx);
				Binding::initialize(false);
				for(auto attr : {attribs...}) {
					i++;
					SDL_GLattr const& k = std::get<0>(attr);
					int const& v0 = std::get<1>(attr);
					int& v1 = std::get<2>(attr);
					SDL_GL_GetAttribute(k, &v1);
					if(errors() || (v0 != v1)) {
						std::ostringstream oss;
						oss << "Attribute #" << i << "/"
							<< sizeof...(S) << ": [" << k << "] = "
							<< v1 << " != " << v0;
						errors.push_back(oss.str());
						break;
					}
				}
				return;
			} while(0);
		}
	};
}

#endif
