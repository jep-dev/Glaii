#ifndef GLSL_HPP
#define GLSL_HPP
#include <SDL2/SDL_video.h>
#include "view.hpp"
#include <iostream>
#include <vector>
#include <map>

namespace View {
	using namespace Abstract;

	namespace Shaders {
		using std::ostream;

		/** @brief RAII shader program with at least one member
		 * @tparam E0 Type of the first shader member
		 * @tparam EN Type of the remaining shader members
		 */
		template<GLenum E0, GLenum... EN> struct Program;

		/** @brief Set and return the value for the given shader.
		 * @param id The SDL-issued shader id
		 * @param k The key for the given value
		 * @param src The initial value to set
		 * @param dest Pointer to the value to assign to (optional)
		 * @return The resulting value, potentially referenced by dest
		 */
		int shaderiv(GLuint id, GLenum k, int src = -1, int *dest = 0);
		/** @brief Set and return the value for the given program.
		 * @param id The SDL-issued shader program id
		 * @param k The key for the given value
		 * @param src The initial value to set
		 * @param dest Pointer to the value to assign to (optional)
		 * @return The resulting value, potentially referenced by dest
		 */
		int programiv(GLuint id, GLenum k, int src = -1, int *dest = 0);

		/** @brief RAII shader alloc/source/compile/dealloc */
		struct Shader {
		protected:
			const GLuint id;
			const GLboolean is_program = glIsProgram(id),
				is_shader = glIsShader(id);
		public:
			operator GLuint(void) const;
			GLint operator()(GLenum i) const;
			bool operator()(GLenum i, int match) const;
			bool source(std::string const& s) const;
			bool compile(void) const;
			bool attach(GLuint s) const;
			bool link(void) const;
			bool validate(void) const;
			string info(void) const;
			Shader(void);
			Shader(GLenum E);
			Shader(GLuint id);
			virtual ~Shader(void);
		};
		template<GLenum E0, GLenum... EN>
		struct Program {
			static constexpr auto N = sizeof...(EN)+1;
			Shader program = {}, shaders[N] = {{E0}, {EN}...};
			const GLenum types[N] = {E0, EN...};
		};
	}

}
#endif
