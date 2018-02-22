#ifndef RELEASE_HPP
#define RELEASE_HPP

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
 * doesn't match GL_[constant] or GL[type] conventions. */
#ifndef GLCTX
#define GLCTX(X) SDL_GL_CONTEXT_ ## X
#endif

#endif
