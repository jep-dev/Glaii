/*! @file app/release.cpp
 *  @brief Prototype for the true entry point; creates a window equipped with
 *  a GL context, acquires and builds shaders, and runs the main event loop
 *  with the bound shaders on a sample model. Features are added here as
 *  proofs of concepts before integration into the appropriate module. */

///@cond
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <SDL.h>
///@endcond

#include "release.hpp"
#include "window.hpp"
#include "glsl.hpp"
#include "streams.hpp"
#include "stopwatch.hpp"

#include "geometry.hpp"
#include "model.hpp"

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
	if(!p.build())
		return dest << "Could not build shader\n" << p.info(), false;

	p.use();
	auto id_mvp = p.uniform("mvp");
	if(id_mvp == -1)
		return dest << "MVP uniform not found!\n", false;

	FSignal res;
	unsigned frame = 0, interval = 60;

	dest << std::setprecision(4);
	auto watch = stopwatch(&perf_rate<float>);
	while(watch.start(), res = win.validate()) {
		res = win.draw(frame, id_mvp);
		watch.pause();
		if(!(frame % interval))
			dest << watch << endl;
		frame++;
	}
	dest << "\nWindow exited; " << res << '\n' << win;
	return res.error == FSignal::Code::quit;
}

int main(int argc, const char *argv[]) {
	using namespace View;
	using std::cout;
	using std::endl;
	using std::flush;
	using std::chrono::duration;
	using std::chrono::seconds;

	int n_frames = 20,
		mods_in = SDL_INIT_VIDEO,
		mods_err, mods_out, run_out;

	if(!(mods_in &= SDL_INIT_EVERYTHING))
		cout << "# Deferring SDL init." << endl;
	else cout << "# Starting SDL... " << flush;

	SDL_Init(mods_in);
	mods_out = SDL_WasInit(0);
	mods_in |= mods_out;           // VIDEO implies EVENTS, etc. so this...
	mods_err = mods_in ^ mods_out; // ... prevents false positives here.

	if(mods_err) {
		cout << (mods_err > 1 ?  "several modules " : "a module ")
			<< "failed to load!" << endl;

		std::ostringstream sdl_oss;
		auto pos = sdl_oss.tellp();
		sdl_oss << SDL_GetError();
		if(pos > sdl_oss.tellp())
			cout << "## SDL: " << sdl_oss.str() << endl;
	} else {
		cout << "done.\n# Beginning test..." << endl;
		auto t0 = std::chrono::system_clock::now();
		run_out = run(cout, 30);
		duration<float> dt = std::chrono::system_clock::now() - t0;
		cout << "# Test " << (run_out ? "passed" : "failed") << " after "
			<< dt.count() << " seconds." << endl;
	}

	SDL_Quit();
}
