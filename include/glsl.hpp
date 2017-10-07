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

		// RAII shaders and program (all Shaders)
		template<GLenum, GLenum...> struct Program;

		/* Set and return the value for the given shader. */
		int shaderiv(GLuint id, GLenum k, int src = -1, int *dest = 0); /* {
			if(!dest) dest = &src;
			return glGetShaderiv(id, k, dest), *dest;
		}*/
		/* Set and return the value for the given program. */
		int programiv(GLuint id, GLenum k, int src = -1, int *dest = 0);/* {
			if(!dest) dest = &src;
			return glGetProgramiv(id, k, dest), *dest;
		} */

		struct Shader {
		protected:
			const GLuint id;
			const GLboolean is_program = glIsProgram(id),
				is_shader = glIsShader(id);
		public:
			operator GLuint(void) const;//{ return id; }
			bool source(std::string const& s) const;/* {
				if(!is_shader) return false;
				auto sc = s.c_str();
				return (glShaderSource(id, 1, &sc, NULL),
					(*this)(GL_SHADER_SOURCE_LENGTH));
			}*/
			bool compile(void) const;/* {
				return is_shader && (*this)(GL_SHADER_SOURCE_LENGTH)
					&& (glCompileShader(id),
						(*this)(GL_COMPILE_STATUS, int(GL_TRUE)));
			}*/
			bool attach(GLuint s) const;/* {
				if(!is_program || !glIsShader(s)) return false;
				return (*this)(GL_ATTACHED_SHADERS) < (glAttachShader(id,s),
					(*this)(GL_ATTACHED_SHADERS));
			}*/
			bool link(void) const;/* {
				return is_program && (glLinkProgram(id),
					(*this)(GL_LINK_STATUS, int(GL_TRUE)));
			}*/
			bool validate(void) const;/* {
				return is_program && (glValidateProgram(id),
					(*this)(GL_VALIDATE_STATUS, int(GL_TRUE)));
			}*/
			std::string info(void) const;/* {
				auto ilen = (*this)(GL_INFO_LOG_LENGTH);
				GLchar buf[ilen+1] = {0};
				if(is_shader) {
					glGetShaderInfoLog(id, ilen, NULL, buf);
				} else if(is_program) {
					glGetProgramInfoLog(id, ilen, NULL, buf);
				} else return "";
				return {buf};
			}*/
			GLint operator()(GLenum i) const;/* {
				return is_shader ? shaderiv(id, i)
					: is_program ? programiv(id, i) : 0;
			}*/
			bool operator()(GLenum i, int match) const;/* {
				return match == (*this)(i);
			}*/
			Shader(void);// Shader(glCreateProgram()) {}
			Shader(GLenum E);// Shader(glCreateShader(E)) {}
			Shader(GLuint id);//: id(id) {}
			virtual ~Shader(void);/* {
				if((*this)(GL_DELETE_STATUS, int(GL_TRUE))) return;
				if(is_shader) glDeleteShader(id);
				else if(is_program) glDeleteProgram(id);
			}*/
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
