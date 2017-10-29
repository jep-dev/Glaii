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
	/*const char *paths[] = {GLSL_VERT, GLSL_FRAG};
	for(auto i = 0; i < 2; i++) {
		Streams::Cutter cut(paths[i]);
		if(!cut) return dest << "Could not read "
			<< paths[i] << '\n', false;
		if(!p[i].source(cut).compile())
			return dest << "Could not compile "
				<< paths[i] << ":\n" << p[i].info(), false;
	}
	if(!p.build() || !p.use())
		return dest << "Could not use shader program\n"
			<< p.info(), false;
	*/
	if(!p.build())
		return dest << "Could not build shader program\n"
			<< p.info(), false;
	p.use();

	auto id_mvp = p.uniform("mvp");
	if(id_mvp == -1)
		return dest << "MVP uniform not found!", false;

	float x0 = -asp, y0 = 1, z0 = -1,
		  x1 = asp, y1 = 10, z1 = 1,
		/* Using y as near/far axis; this retains handedness but makes
		 * forward, right, and up axes positive in default orientation */
		m00 = y0/x0, m11 = y0/y1, m2_ = y0-y1,
		m22 = (y0+y1)/m2_, m23 = 2*y0*y1/m2_,
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
	auto tStart = SDL_GetPerformanceCounter(), t0 = tStart;
	endl(dest << "Frame" << right
		<< setw(9) << setfill('.') << "FPS");
	while(win) {
		if(!win.draw(frame)) break;
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
