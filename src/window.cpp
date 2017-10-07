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
		if(!src.errors) return dest;
		return dest << "Window: " << src.errors;
	}
	template<GLenum E0, GLenum... EN>
	Window& Window::operator<<(Shaders::Program<E0,EN...> const& p) {
		using namespace Shaders;
		Shader const& prog = p.program;
		do {
			if(!prog.is_program) break;
			bool cont = true;
			for(GLenum iv : {GL_LINK_STATUS, GL_ATTACHED_SHADERS})
				if(!prog(iv, int(GL_TRUE))) {
					endl(std::cout << "Invalid shader program");
					cont = false;
					break;
				}
			if(cont) glUseProgram(prog);
		} while(0);
		errors();
		return *this;
	}
	bool Window::validate(void) {
		live &= !errors();
		return live;
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
	bool Window::update(void) {
		if(!validate()) return false;
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
			if(!handle(ev)) return false;
		return validate();
	}
	bool Window::draw(void) {
		if(!update()) return false;
		SDL_GL_SwapWindow(win);
		SDL_Delay(150);
		return validate();
	}


}
