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

/* TODO Remove convenience macro? GL prefix is more descriptive but
 * doesn't match GL_[constant] or GL[type] conventions */
#ifndef GLCTX
#define GLCTX(X) SDL_GL_CONTEXT_ ## X
#endif

bool run(std::ostream &dest, int n_frames) {
	using namespace View;
	using namespace Shaders;
	using std::left;
	using std::right;
	using std::setw;
	using std::setfill;
	using std::flush;
	using std::endl;
	using std::setprecision;

	unsigned w = 640, h = 480;
	auto asp = float(w)/h;
	Window win("Title", w, h, SDL_WINDOW_RESIZABLE, {
		{GLCTX(PROFILE_MASK), GLCTX(PROFILE_CORE)},
		{GLCTX(MAJOR_VERSION), 3}, {GLCTX(MINOR_VERSION), 3},
		{SDL_GL_DOUBLEBUFFER, 1}, {SDL_GL_DEPTH_SIZE, 24}
	});
	if(!win) return dest << win, false;

	Streams::Cutter c0(GLSL_VERT), c1(GLSL_FRAG);
	Program<GL_VERTEX_SHADER, GL_FRAGMENT_SHADER> p {c0, c1};
	if(!p.build())
		return dest << "Could not build shader program\n"
			<< p.info(), false;
	p.use();

	auto id_mvp = p.uniform("mvp");
	if(id_mvp == -1)
		return dest << "MVP uniform not found!", false;

	unsigned frame = 0, window = 30;
	float rollsum = 0, store[window] = {0};
	auto tStart = SDL_GetPerformanceCounter(), t0 = tStart;
	endl(dest << "FPS during " << window << " frames:");
	dest << setprecision(5);
	while(win) {
		if(!win.draw(frame, id_mvp)) break;
		auto t1 = SDL_GetPerformanceCounter();
		auto index = ++frame % window;
		auto freq = SDL_GetPerformanceFrequency();
		auto fps = float(freq)/(t1-t0);
		rollsum += fps - store[index];
		store[index] = fps;
		if((frame > window) && !(frame % window)) {
			float mean = rollsum/window, dev = 0, dev2 = 0;
			for(unsigned i = 0; i < window; i++) {
				auto diff = mean - store[i];
				dev2 += diff*diff;
			}
			dev = sqrt(dev2/window);
			dest << "Average: " << setw(6) << mean
				<< "; std. deviation: " << dev << "\r";
			(frame % (window*8)) ? flush(dest) : endl(dest);
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
