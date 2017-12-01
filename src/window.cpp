#include "view.hpp"

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
	bool Window::handle(SDL_QuitEvent const& ev) {
		return false;
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
	bool Window::update(unsigned frame) {
		if(!validate()) return false;
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
			if(!call_handler(*this, ev)) return false;
		return validate();
	}
	bool Window::draw(unsigned frame, GLint id_mvp) {
		static constexpr unsigned mspf60 = 100/6+1;
		if(id_mvp == -1) return false;
		if(!update(frame)) return false;
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		/* From app/release.cpp */
		// TODO Move to src/model.cpp or sub
		static unsigned w = 640, h = 480;
		static auto asp = float(w)/h;
		static float
			x0 = -asp, y0 = -1, z0 =  1,
			x1 =  asp, y1 =  1, z1 = 10,
			m00 =    2*z0/(x1-x0), m02 = (x0+x1)/(x1-x0),
			m11 =    2*z0/(y1-y0), m12 = (y0+y1)/(y1-y0),
			m22 = (z0+z1)/(z0-z1), m23 = 2*z0*z1/(z0-z1),
			mvp[] = {
				m00,  0,   0,   0,
				 0, m11,   0,   0,
				 0,   0, m22, m23,
				 0,   0,  -1,   0
			}, vertices[] = {
				-1,  -1,  -2,  +1,
				+1,  -1,  -2,  +1,
				+1,  +1,  -2,  +1,
				-1,  +1,  -2,  +1
			};

		static unsigned indices[] = {
			0, 1, 2, 0, 3, 2
		};

		/* From app/release.cpp */
		// TODO Move to sub
		static bool once = false;
		static GLuint vbo = 0, vao = 0;
		if(!once) {
			once = true;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof vertices,
				vertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		}
		glUniformMatrix4fv(id_mvp, 1, GL_FALSE, mvp);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, sizeof indices,
			GL_UNSIGNED_INT, indices);
		glDisableVertexAttribArray(0);

		SDL_GL_SwapWindow(win);
		SDL_Delay(mspf60);
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
				SDL_WINDOWPOS_CENTERED, w, h, flags | SDL_WINDOW_OPENGL
				| SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
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
