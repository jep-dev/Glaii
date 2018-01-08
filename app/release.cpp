/*! @file app/release.cpp
 *  @brief Prototype for the true entry point; creates a window equipped with
 *  a GL context, acquires and builds shaders, and runs the main event loop
 *  with the bound shaders on a sample model. Features are added here as
 *  proofs of concepts before integration into the appropriate module. */

///@cond
#include <fstream>
#include <iomanip>
#include <iostream>
#include <SDL2/SDL.h>
///@endcond

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
	using std::setw;
	using std::flush;
	using std::endl;

	unsigned w = 640, h = 480;
	auto asp = float(w)/h;
	Window win("Title", w, h, SDL_WINDOW_RESIZABLE, {
		{GLCTX(PROFILE_MASK), GLCTX(PROFILE_CORE)},
		{GLCTX(MAJOR_VERSION), 3}, {GLCTX(MINOR_VERSION), 3},
		{SDL_GL_DOUBLEBUFFER, 1}, {SDL_GL_DEPTH_SIZE, 24}
	});
	if(!win) {
		dest << win;
		return false;
	}

	Streams::Cutter c0(GLSL_VERT), c1(GLSL_FRAG);
	Program<GL_VERTEX_SHADER, GL_FRAGMENT_SHADER> p {c0, c1};
	if(!p.build()) {
		dest << "Could not build shader program\n"
			<< p.info();
		return false;
	}
	p.use();

	auto id_mvp = p.uniform("mvp");
	if(id_mvp == -1) {
		dest << "MVP uniform not found!";
		return false;
	}

	unsigned frame = 0, samples = 60;
	float rollsum = 0, store[samples] = {0};
	auto tStart = SDL_GetPerformanceCounter(), t0 = tStart;
	endl(dest << "FPS during " << samples << " frames:");
	dest << std::setprecision(5);
	FSignal res = {0};
	while(win) {
		res = win.draw(frame, id_mvp);
		if(!res) break;
		auto t1 = SDL_GetPerformanceCounter();
		auto index = ++frame % samples;
		auto freq = SDL_GetPerformanceFrequency();
		auto fps = float(freq)/(t1-t0);
		rollsum += fps - store[index];
		store[index] = fps;
		if((frame > samples) && !(frame % samples)) {
			float mean = rollsum/samples, dev = 0, dev2 = 0;
			for(unsigned i = 0; i < samples; i++) {
				auto diff = mean - store[i];
				dev2 += diff*diff;
			}
			dev = sqrt(dev2/samples);
			dest << "Average: " << setw(6) << mean
				<< "; std. deviation: " << dev << "\r";
			(frame % (samples*8)) ? flush(dest) : endl(dest);
		}
		t0 = t1;
	}
	dest << "Window exited with status " << (unsigned)res.error
		<< " (living=0, failing=1, quitting=2)\n" << win;
	// 'Validate' step still necessary as a post-mortem, since false means
	// both 'quit' and 'fail' -- see note in src/Window.cpp
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
