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
		return m_live && m_win && m_ctx && !m_errors;
	}
	Window::operator SDL_GLContext const(void) const {
		return m_ctx;
	}
	Window::operator SDL_Window *const(void) const {
		return m_win;
	}
	std::ostream& operator<<(std::ostream& dest, Window const& src) {
		if(!(src.m_errors)) return dest;
		return dest << "Window: " << src.m_errors;
	}
	bool Window::validate(void) {
		return m_live && (m_live &= !m_errors());
	}
	bool Window::handle(SDL_QuitEvent const& ev) {
		return false;
	}
	bool Window::handle(SDL_WindowEvent const& ev) {
		//m_width = 1; m_height = 1;
		switch(ev.type) {
			case SDL_WINDOWEVENT_CLOSE: return false;
			case SDL_WINDOWEVENT_RESIZED: {
				int p1 = 0, p2 = 0;
				SDL_GetWindowSize(m_win, &p1, &p2);
				if(!p1 || !p2) return false;
				if(p1 != ev.data1 || p2 != ev.data2) return false;
				m_width = ev.data1;
				m_height = ev.data2;
				glViewport(0, 0, m_width, m_height);
				break;
			} default: return true;
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

		int l_width = 0, l_height = 0;
		SDL_GetWindowSize(m_win, &l_width, &l_height);
		if(l_width <= 0 || l_height <= 0) return false;
		glViewport(0, 0, l_width, l_height);
		m_width = l_width;
		m_height = l_height;

		// FOVy is fixed; FOVx compensates for aspect ratio
		float asp = float(m_width)/m_height,
			x0 = -asp, x1 = asp, dx = x1-x0,
			y0 =   -1, y1 =   1, dy = y1-y0,
			z0 =    1, z1 =  10, dz = z0-z1,
			mx =    2*z0/dx, my =    2*z0/dy, mz = (z0+z1)/dz,
			ax = (x0+x1)/dx, ay = (y0+y1)/dy, tz = 2*z0*z1/dz,
			mvp[] = {
				mx,  0,  0,  0,
				 0, my,  0,  0,
				ax, ay, mz, -1,
				 0,  0, tz,  0
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
				vertices, GL_DYNAMIC_DRAW);

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

		SDL_GL_SwapWindow(m_win);
		SDL_Delay(mspf60);
		return validate();
	}

	Window::Window(const char *title, int w, int h,
			Uint32 flags, std::map<SDL_GLattr, int> const& attribs) {
		do {
			/* TODO (high) - reify errors, create handlers with policies
			 * For example, GL version mismatch is almost certainly critical,
			 * whereas bit depth, double buffering, etc. definitely aren't.
			 * Reg. policies: use of fail bit implies that the object
			 * cannot be used, but state is preserved until destruction,
			 * as if it can be repaired or scrapped - but these require an
			 * autopsy despite the fact that the flag was set due to a
			 * known cause of death!
			 */
			if(m_errors()) {
				m_errors.emplace_back("Window creation disabled.");
				break;
			}
			int i = 0;
			for(auto const& attr : attribs)
				SDL_GL_SetAttribute(attr.first, attr.second);
			m_win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED, w, h, flags | SDL_WINDOW_OPENGL
				| SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
			if(m_errors() || !m_win) {
				m_errors.emplace_back("SDL window could not be created.");
				break;
			}
			m_ctx = SDL_GL_CreateContext(m_win);
			if(m_errors() || !m_ctx) {
				m_errors.emplace_back("SDL context could not be created.");
				break;
			}
			int l_width, l_height;
			SDL_GetWindowSize(m_win, &l_width, &l_height);
			m_width = l_width;
			m_height = l_height;
			SDL_GL_MakeCurrent(m_win, m_ctx);
			Binding::initialize(false);
			for(auto const& attr : attribs) {
				i++;
				SDL_GLattr k = attr.first;
				int v0 = attr.second, v1 = v0;
				SDL_GL_GetAttribute(k, &v1);
				if(m_errors() || (v0 != v1)) {
					std::ostringstream oss;
					oss << "Attribute #" << i << ": ["
						<< k << "] = " << v1 << " != " << v0;
					m_errors.emplace_back(oss.str());
					break;
				}
			}
			m_live = true;
			return;
		} while(0);
		m_live = false;
	}

}
