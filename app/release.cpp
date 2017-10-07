#include <fstream>
#include <iomanip>
#include <iostream>
#include <SDL2/SDL.h>

#include "glsl.hpp"
#include "window.hpp"

bool run(std::ostream &dest, int n_frames) {
	using namespace View;
	using namespace Shaders;
	static constexpr auto
			iv_cmp = GL_COMPILE_STATUS, iv_link = GL_LINK_STATUS,
			iv_del = GL_DELETE_STATUS,  iv_log = GL_INFO_LOG_LENGTH;

	ErrorFIFO fifo;
	Window win("Title", 640, 480, SDL_WINDOW_RESIZABLE,
		attrib(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE),
		attrib(SDL_GL_CONTEXT_MAJOR_VERSION, 3),
		attrib(SDL_GL_CONTEXT_MAJOR_VERSION, 3),
		attrib(SDL_GL_DOUBLEBUFFER, 1));
	if(!win) return false;

	Program<GL_VERTEX_SHADER, GL_FRAGMENT_SHADER> p;
	auto &prog = p.program;
	const char *fnames[] = {"share/default.vert", "share/default.frag"};
	int cont = true;
	for(auto i = 0; i < 2; i++) {
		auto& shader = p.shaders[i];
		Source src = {fnames[i]};
		if(!(cont = shader.source(src.data)))
			endl(dest << "Could not use source " << fnames[i]);
		else if(!(cont = shader.compile()))
			endl(dest << "Could not compile " << fnames[i]);
		else cont = prog.attach(shader);
		auto info = shader.info();
		if(info.size()) endl(dest << info);
		if(!cont) break;
	}
	if(cont) {
		cont = prog.link() && prog.validate();
		auto info = prog.info();
		if(info.size()) endl(dest << info);
	}
	if(cont) glUseProgram(prog);
	if(fifo()) endl(dest << fifo);
	flush(dest << "Rendering...");
	for(auto i = 0; i < n_frames; i++)
		if(!(cont = win.draw())) {
			endl(dest << " exited after " << (i+1) << '/'
				<< (n_frames) << " frames.");
			break;
		}
	dest << win;
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
		cout << ' ' << (run(cout, 30) ? "Passed" : "Failed")
			<< " test." << endl;
	} else init_err = 1, cout << "abort." << endl;
	SDL_Quit();
	return ((init_err << 1) | run_err);
}
