#include "window.hpp"

#include <SDL2/SDL.h>

#include <glbinding/Binding.h>
#include <glbinding/Meta.h>
#include <glbinding/Version.h>
#include <glbinding/ContextInfo.h>

namespace View {
	using namespace gl;
	using namespace glbinding;

	Window::operator bool(void) const {
		return live && win && ctx && !errors;
	}
	Window::operator SDL_GLContext const(void) const {
		return ctx;
	}
	Window::operator SDL_Window *const(void) const {
		return win;
	}
	std::ostream& operator<<(std::ostream& dest, Window const& src) {
		if(!(src.errors)) return dest;
		return dest << "Window: " << src.errors;
	}
	bool Window::validate(void) {
		return live && (live &= !errors());
	}
	bool Window::handle(SDL_WindowEvent const& ev) {
		switch(ev.type) {
			case SDL_WINDOWEVENT_CLOSE: return false;
			default: return true;
		}
	}
	bool Window::handle(SDL_KeyboardEvent const& ev) {
		switch(ev.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE: return false;
			default: return true;
		}
	}
	bool Window::handle(SDL_Event const& ev) {
		switch(ev.type) {
			case SDL_WINDOWEVENT: return handle(ev.window);
			case SDL_KEYDOWN: return handle(ev.key);
			case SDL_QUIT: return false;
			default: return true;
		}
	}
	bool Window::update(unsigned frame) {
		if(!validate()) return false;
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
			if(!handle(ev)) return false;
		return validate();
	}
	bool Window::draw(unsigned frame) {
		static constexpr unsigned freq60 = 100/6;
		if(!update(frame)) return false;
		SDL_GL_SwapWindow(win);
		SDL_Delay(freq60);
		return validate();
	}

	Window::Window(const char *title, int w, int h,
			Uint32 flags, std::map<SDL_GLattr, int> const& attribs) {
		do {
			if(errors()) {
				errors.emplace_back("Window creation disabled.");
				break;
			}
			int i = 0;
			for(auto const& attr : attribs)
				SDL_GL_SetAttribute(attr.first, attr.second);
			win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED, w, h, flags | SDL_WINDOW_OPENGL);
			if(errors() || !win) {
				errors.emplace_back("SDL window could not be created.");
				break;
			}
			ctx = SDL_GL_CreateContext(win);
			if(errors() || !ctx) {
				errors.emplace_back("SDL context could not be created.");
				break;
			}
			SDL_GL_MakeCurrent(win, ctx);
			Binding::initialize(false);
			for(auto const& attr : attribs) {
				i++;
				SDL_GLattr k = attr.first;
				int v0 = attr.second, v1 = v0;
				SDL_GL_GetAttribute(k, &v1);
				if(errors() || (v0 != v1)) {
					std::ostringstream oss;
					oss << "Attribute #" << i << ": ["
						<< k << "] = " << v1 << " != " << v0;
					errors.emplace_back(oss.str());
					break;
				}
			}
			/* TODO investigate unsupported swap interval, decide on
			 * a policy for missing swap interval (heuristic delays?) */
			/*if(!SDL_GL_SetSwapInterval(1)) {
				errors();
				errors.emplace_back("Swap interval / VSYNC not supported!");
				break;
			}*/
			live = true;
			return;
		} while(0);
		live = false;
	}

}
