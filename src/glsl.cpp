#include "view.hpp"

namespace View {
namespace Shaders {
	/*GLint queryIv(GLuint id, GLenum k, GLint *pdest,
			void (*fnQuery) (GLuint, GLenum, GLint*)) {
		GLint dest = -1;
		if(!pdest) pdest = &dest;
		(*fnQuery) (id, k, pdest);
		return *pdest;
	}*/
	GLint queryIv(GLuint id, GLenum k, GLint *pdest) {
		if(glIsShader(id)) return shaderIv(id, k, pdest);
		if(glIsProgram(id)) return programIv(id, k, pdest);
		return -1;
	}
	GLint shaderIv(GLuint id, GLenum k, GLint *pdest) {
		GLint dest = -1;
		if(!pdest) pdest = &dest;
		glGetShaderiv(id, k, pdest);
		return *pdest;
	}
	GLint programIv(GLuint id, GLenum k, GLint *pdest) {
		GLint dest = -1;
		if(!pdest) pdest = &dest;
		glGetProgramiv(id, k, pdest);
		return *pdest;
	}
	bool queryAssertIv(GLuint id, GLenum key, GLint val) {
		return val == queryIv(id, key);
	}
	bool shaderAssertIv(GLuint id, GLenum key, GLint val) {
		return val == shaderIv(id, key);
	}
	bool programAssertIv(GLuint id, GLenum key, GLint val) {
		return val == programIv(id, key);
	}
	std::string programInfo(GLuint id) {
		auto len = programIv(id, GL_INFO_LOG_LENGTH);
		GLchar buf[len];
		glGetProgramInfoLog(id, len, 0, buf);
		return buf;
		//return std::string(buf);
	}
	std::string shaderInfo(GLuint id) {
		auto len = shaderIv(id, GL_INFO_LOG_LENGTH);
		GLchar buf[len];
		glGetShaderInfoLog(id, len, 0, buf);
		return buf;
		//return std::string(buf);
	}
	std::string queryInfo(GLuint id) {
		if(glIsProgram(id)) return programInfo(id);
		if(glIsShader(id)) return shaderInfo(id);
		return "(no info available)";
	}

	Shader::operator GLuint(void) const {
		return m_id;
	}
	bool Shader::build(void) const {
		if(m_isShader) {
			if(shaderAssertIv(m_id, GL_SHADER_SOURCE_LENGTH, 0))
				return false;
			glCompileShader(m_id);
			return shaderAssertIv(m_id,
				GL_COMPILE_STATUS, GLint(GL_TRUE));
		}
		if(m_isProgram) {
			if(programAssertIv(m_id, GL_ATTACHED_SHADERS, 0))
				return false;
			glLinkProgram(m_id);
			return programAssertIv(m_id, GL_LINK_STATUS, GLint(GL_TRUE));
		}
		return false;
	}
	std::string Shader::info(void) const {
		return queryInfo(m_id);
	}

	/*Shader const& Shader::source(std::string const& s) const {
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
	}*/
	Shader::Shader(void): m_id(glCreateProgram()) {}
	Shader::Shader(GLenum E): m_id(glCreateShader(E)) {}
	Shader::Shader(GLenum E, std::string const& src):
			m_id(glCreateShader(E)) {
		if(!src.size()) return;
		auto szSrc = src.c_str();
		glShaderSource(m_id, 1, &szSrc, 0);
	}
	Shader::~Shader(void) {
		if(queryAssertIv(m_id, GL_DELETE_STATUS, GLint(GL_TRUE))) return;
		if(m_isShader) glDeleteShader(m_id);
		else if(m_isProgram) glDeleteProgram(m_id);
	}
}
}
