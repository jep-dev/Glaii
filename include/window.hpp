/** @file include/window.hpp
 * @brief Declares the Window and context for rendering */

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "events.hpp"

///@cond
#include <map>
#include <glbinding/Binding.h>
///@endcond

namespace View {
	using namespace gl;
	using namespace glbinding;
	struct Window;
}

#include "runnable.hpp"

namespace View {

	//template struct Abstract::Handler_t<Window>;
	/** @brief RAII and operations on an SDL window/context pair. */
	struct Window:
		Abstract::Derived_t<Window>,
		Abstract::Updatable_t<Window>,
		/*Abstract::Handler_t<Window, SDL_QuitEvent, SDL_WindowEvent,
			SDL_MouseButtonEvent, SDL_MouseMotionEvent, SDL_KeyboardEvent>*/
		Abstract::Handler_t<Window> {
	protected:
		SDL_Window *m_win;
		Streams::ErrorFIFO m_errors;
		SDL_GLContext m_ctx;
		bool m_live;
	public:
		unsigned m_width, m_height;
		operator bool(void) const;
		operator SDL_Window *const(void) const;
		operator SDL_GLContext const(void) const;

		friend std::ostream&
		operator<<(std::ostream &dest, Window const& src);
		FSignal validate(void);
		template<typename T> FSignal handle(T const& ev);

		/*FSignal handle(SDL_QuitEvent const& ev);
		FSignal handle(SDL_WindowEvent const& ev);
		FSignal handle(SDL_MouseButtonEvent const& ev);
		FSignal handle(SDL_MouseMotionEvent const& ev);
		FSignal handle(SDL_KeyboardEvent const& ev);*/

		FSignal update(unsigned frame);
		FSignal draw(unsigned frame, GLint id_mvp);

		Window(const char *title, int w, int h,
			Uint32 flags, std::map<SDL_GLattr, int> const& attribs);
	};
	template<typename T>
	FSignal Window::handle(T const& t) {
		return {FSignal::Code::ok};
	}
}

#endif
