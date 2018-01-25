/** @file include/window.hpp
 * @brief Declares the Window and context for rendering */

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "view.hpp"
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
#include "view.hpp"

namespace View {
	using Abstract::FSignal;

	/** @brief RAII and operations on an SDL window/context pair. */
	struct Window:
		Abstract::Updatable_t<Window>,
		Abstract::Handler_t<Window> {
	protected:
		FSignal m_live;
		SDL_Window *m_win;
		SDL_GLContext m_ctx;
		Streams::ErrorFIFO m_errors;
	public:
		unsigned m_width, m_height;
		//operator bool(void) const;
		operator SDL_Window *const(void) const;
		operator SDL_GLContext const(void) const;

		friend std::ostream&
		operator<<(std::ostream &dest, Window const& src);
		FSignal validate(void);
		template<typename T> FSignal handle(T const& ev);

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
