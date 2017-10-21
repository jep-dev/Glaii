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

	unsigned long sumDt = 0;
	unsigned frame = 0, frameField = 6, fpsField = 7, diffField = 8;
	auto tStart = SDL_GetPerformanceCounter(), t0 = tStart;
	endl(dest << std::left << std::setw(frameField) << "Frame" << ' '
		<< std::setw(fpsField) << "FPS" << " FPS/avg-1");
	while(win) {
		if(!win.draw(frame)) break;
		auto t1 = SDL_GetPerformanceCounter(),
			dt = t1 - t0;
		t0 = t1;
		sumDt += dt;
		frame++;
		if((frame & 7) != 0) continue;
		auto curFps = float(SDL_GetPerformanceFrequency())/dt,
			 diffFps = 100*(float(sumDt)/frame/dt - 1);
		flush(dest << '\r' << std::left
			<< std::setw(frameField) << frame << ' '
			<< std::setw(fpsField) << int(curFps*1000)/1000.f << ' '
			<< std::setw(diffField) << std::showpos
			<< int(diffFps*1000)/1000.f << std::noshowpos << std::right);
		if((frame & 255) == 0) endl(dest);
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
