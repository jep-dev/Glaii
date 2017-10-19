#include "glsl.hpp"

namespace View {
namespace Shaders {
	int shaderiv(GLuint id, GLenum k, int src, int *dest) {
		if(!dest) dest = &src;
		return glGetShaderiv(id, k, dest), *dest;
	}
	int programiv(GLuint id, GLenum k, int src, int *dest) {
		if(!dest) dest = &src;
		return glGetProgramiv(id, k, dest), *dest;
	}
	Shader::operator GLuint(void) const { return id; }
	Shader const& Shader::source(std::string const& s) const {
		if(!is_shader) return *this;
		auto sc = s.c_str();
		glShaderSource(id, 1, &sc, NULL);
		return *this;
	}
	bool Shader::compile(void) const {
		return is_shader && (*this)(GL_SHADER_SOURCE_LENGTH)
			&& (glCompileShader(id),
				(*this)(GL_COMPILE_STATUS, int(GL_TRUE)));
	}
	bool Shader::attach(GLuint s) const {
		if(!is_program || !glIsShader(s)) return false;
		auto pre = (*this)(GL_ATTACHED_SHADERS);
		glAttachShader(id, s);
		return (*this)(GL_ATTACHED_SHADERS, pre + 1);
	}
	bool Shader::link(void) const {
		return is_program && (glLinkProgram(id),
			(*this)(GL_LINK_STATUS, int(GL_TRUE)));
	}
	bool Shader::validate(void) const {
		return is_program && (glValidateProgram(id),
			(*this)(GL_VALIDATE_STATUS, int(GL_TRUE)));
	}
	std::string Shader::info(void) const {
		auto ilen = (*this)(GL_INFO_LOG_LENGTH);
		GLchar buf[ilen+1] = {0};
		if(is_shader) {
			glGetShaderInfoLog(id, ilen, NULL, buf);
		} else if(is_program) {
			glGetProgramInfoLog(id, ilen, NULL, buf);
		} else return "";
		return {buf};
	}
	GLint Shader::operator()(GLenum i) const {
		return is_shader ? shaderiv(id, i)
			: is_program ? programiv(id, i) : 0;
	}
	bool Shader::operator()(GLenum i, int match) const {
		return match == (*this)(i);
	}
	Shader::Shader(void): Shader(glCreateProgram()) {}
	Shader::Shader(GLenum E): Shader(glCreateShader(E)) {}
	Shader::Shader(GLuint id): id(id) {}
	Shader::~Shader(void) {
		if((*this)(GL_DELETE_STATUS, int(GL_TRUE))) return;
		if(is_shader) glDeleteShader(id);
		else if(is_program) glDeleteProgram(id);
	}
}
}
