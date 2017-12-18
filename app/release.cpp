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

	unsigned frame = 0, samples = 60;
	float rollsum = 0, store[samples] = {0};
	auto tStart = SDL_GetPerformanceCounter(), t0 = tStart;
	endl(dest << "FPS during " << samples << " frames:");
	dest << std::setprecision(5);
	while(win) {
		if(!win.draw(frame, id_mvp)) break;
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
