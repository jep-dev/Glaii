#include <fstream>
#include <iomanip>
#include <iostream>
#include <SDL2/SDL.h>

#include "glsl.hpp"
#include "window.hpp"

#include "models.hpp"
#include "geometry.hpp"

#ifndef GLSL_ROOT
#define GLSL_ROOT "share/"
#endif

#ifndef GLSL_VERT
#define GLSL_VERT GLSL_ROOT "default.vert"
#endif

#ifndef GLSL_FRAG
#define GLSL_FRAG GLSL_ROOT "default.frag"
#endif

#ifndef CTX
#define CTX(X) SDL_GL_CONTEXT_ ## X
#endif

bool run(std::ostream &dest, int n_frames) {
	using namespace View;
	using namespace Shaders;
	static constexpr auto
		iv_cmp = GL_COMPILE_STATUS, iv_link = GL_LINK_STATUS,
		iv_del = GL_DELETE_STATUS,  iv_log = GL_INFO_LOG_LENGTH;
	static constexpr auto ctr = SDL_GetPerformanceCounter,
		freq = SDL_GetPerformanceFrequency;

	Window win("Title", 640, 480, SDL_WINDOW_RESIZABLE,
		attrib(CTX(PROFILE_MASK), CTX(PROFILE_CORE)),
		attrib(CTX(MAJOR_VERSION), 3), attrib(CTX(MINOR_VERSION), 3),
		attrib(SDL_GL_DOUBLEBUFFER, 1));
	if(!win) {
		dest << win;
		return false;
	}

	Program<GL_VERTEX_SHADER, GL_FRAGMENT_SHADER> p;
	if(p[0].source(Source(GLSL_VERT)).compile()
			&& p[1].source(Source(GLSL_FRAG)).compile())
		win << p;
	else return dest << p.info(), false;

		//bool test = update(win);
	unsigned frame = 0;
	while(win) {
		//bool test = update(win, frame);
		//return test;
		auto pre = (*ctr)();
		if(!win.draw(frame)) break;
		auto post = (*ctr)();
		auto tps = (*freq)();
		auto dt = post - pre;
		if(dt <= 0) continue;
		auto fps = tps / float(dt);
		flush(dest << "\rFPS[" << frame << "] = " << fps);
		frame++;
	}
	dest << '\n' << win;
	return win.validate();
}

int main(int argc, const char *argv[]) {
	using namespace View;
	using std::cout;
	using std::endl;

	int init = 0, init_err = 0, ran = 0, run_err = 0,
		modules = SDL_INIT_VIDEO, n_frames = 20,
		started = (SDL_Init(modules), SDL_WasInit(0) & modules);
	if(init = (modules == started)) {
		cout << "Beginning test." << endl;
		cout << (run(cout, 30) ? "Passed" : "Failed")
			<< " test." << endl;
	} else init_err = 1, cout << "abort." << endl;
	SDL_Quit();
	return ((init_err << 1) | run_err);
}
