#include <fstream>
#include <iomanip>
#include <iostream>
#include <SDL2/SDL.h>

#include "glsl.hpp"
#include "window.hpp"

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

	unsigned w = 640, h = 480;
	auto asp = float(w)/h; // TODO test aspect easing from view.hpp
	Window win("Title", w, h, SDL_WINDOW_RESIZABLE,
			{{CTX(PROFILE_MASK), CTX(PROFILE_CORE)},
			{CTX(MAJOR_VERSION), 3}, {CTX(MINOR_VERSION), 3},
			{SDL_GL_DOUBLEBUFFER, 1}, {SDL_GL_DEPTH_SIZE, 24}});
	if(!win) return dest << win, false;

	Program<GL_VERTEX_SHADER, GL_FRAGMENT_SHADER> p;
	if(p[0].source(Source(GLSL_VERT)).compile()
			&& p[1].source(Source(GLSL_FRAG)).compile())
		win << p;
	else return dest << p.info(), false;

	auto id_mvp = p.uniform("mvp");
	if(id_mvp == -1)
		return dest << "MVP uniform not found!", false;

	float x0 = -asp, y0 = 1, z0 = -1,
		  x1 = asp, y1 = 10, z1 = 1,
		m00 = z0/x0, m11 = z0/z1, m2_ = z0-z1,
		m22 = (z0+z1)/m2_, m23 = 2*z0*z0/m2_,
		mvp[] = {
			m00,  0,   0,   0,
			 0,  m11,  0,   0,
			 0,   0,  m22, m23,
			 0,   0,  -1,   0
		}, vertices[] = {
			-1,  +5,  -1,  +1,
			+1,  +5,  -1,  +1,
			+1,  +5,  +1,  +1,
			-1,  +5,  +1,  +1
		};
	unsigned indices[] = {0, 1, 2, 0, 2, 3};

	GLuint vbo = 0, vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices,
		vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	unsigned frame = 0;
	unsigned long total = 0;
	auto t0 = SDL_GetPerformanceCounter();
	while(win) {
		if(!win.draw(frame)) break;
		auto t1 = SDL_GetPerformanceCounter(),
			dt = t1 - t0;
		t0 = t1;
		total += dt;

		/* Theoretical divide by zero later if timer resolution is not
		 * high enough to register a difference between frames
		 * Realistically the calls to the high-resolution timer alone
		 * should take a few milliseconds -- TODO research? */
		if(dt <= 0) continue;
		auto fps = float(SDL_GetPerformanceFrequency()) / dt,
			fpsRatio = total * 100.0f / (dt * frame);
		flush(dest << "\rFrame " << frame << ": FPS = "
			<< std::setw(4) << fps
			<< " (" << fpsRatio << "% of average)");
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
