#include <fstream>
#include <iomanip>
#include <iostream>
#include <SDL2/SDL.h>

#include "view.hpp"
#include "streams.hpp"

#include "geometry.hpp"
#include "model.hpp"

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
	using std::left;
	using std::right;
	using std::setw;
	using std::setfill;

	unsigned w = 640, h = 480;
	auto asp = float(w)/h; // TODO test aspect easing from view.hpp
	Window win("Title", w, h, SDL_WINDOW_RESIZABLE, {
		{CTX(PROFILE_MASK), CTX(PROFILE_CORE)},
		{CTX(MAJOR_VERSION), 3}, {CTX(MINOR_VERSION), 3},
		{SDL_GL_DOUBLEBUFFER, 1}, {SDL_GL_DEPTH_SIZE, 24}
	});
	if(!win) return dest << win, false;

	// TODO Reduce to Program<GLenum...>(path...)
	Streams::Cutter c0(GLSL_VERT), c1(GLSL_FRAG);
	Program<GL_VERTEX_SHADER, GL_FRAGMENT_SHADER> p {c0, c1};
	if(!p.build())
		return dest << "Could not build shader program\n"
			<< p.info(), false;
	p.use();

	auto id_mvp = p.uniform("mvp");
	if(id_mvp == -1)
		return dest << "MVP uniform not found!", false;

	unsigned frame = 0;
	auto tStart = SDL_GetPerformanceCounter(), t0 = tStart;
	endl(dest << "Frame" << right
		<< setw(9) << setfill('.') << "FPS");
	while(win) {
		if(!win.draw(frame, id_mvp)) break;
		auto t1 = SDL_GetPerformanceCounter();
		frame++;
		if(!(frame & 15)) {
			auto freq = SDL_GetPerformanceFrequency();
			auto fps = float(freq)/(t1-t0);
			flush(dest << '\r'
				<< setw(5) << setfill('.') << left << frame
				<< setw(9) << right << fps << setfill(' '));
			if((frame & 255) == 0) dest << '\n';
		}
		t0 = t1;
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
