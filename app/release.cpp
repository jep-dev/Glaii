/*! @file app/release.cpp
 *  @brief Prototype for the true entry point; creates a window equipped with
 *  a GL context, acquires and builds shaders, and runs the main event loop
 *  with the bound shaders on a sample model. Features are added here as
 *  proofs of concepts before integration into the appropriate module. */

///@cond
#include <fstream>
#include <iomanip>
#include <iostream>
#include <SDL.h>
///@endcond

#include "window.hpp"
#include "glsl.hpp"
#include "streams.hpp"
#include "stopwatch.hpp"

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

	if(!win.validate()) return dest << win, false;

	Streams::Cutter c0(GLSL_VERT), c1(GLSL_FRAG);
	Program<GL_VERTEX_SHADER, GL_FRAGMENT_SHADER> p {c0, c1};
	if(!p.build()) return dest << "Could not build shader program\n"
			<< p.info(), false;

	p.use();
	auto id_mvp = p.uniform("mvp");
	if(id_mvp == -1) return dest << "MVP uniform not found!", false;

	FSignal res;
	unsigned frame = 0, interval = 60;

	dest << std::setprecision(4);
	auto watch = stopwatch(&perf_rate<float>);
	while(watch.start(), res = win.validate()) {
		res = win.draw(frame, id_mvp);
		watch.pause();
		if(!(frame % interval))
			std::cout << watch << std::endl;
		frame++;
	}
	dest << "\nWindow exited; " << res << '\n' << win;
	return res.error == FSignal::Code::quit;
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
