#ifndef GLSL_TPP
#define GLSL_TPP

/*! @file include/glsl.tpp
 *  @brief Implementations from declarations in glsl.hpp */

namespace View {
	namespace Shaders {
		template<GLenum E0, GLenum... EN>
		GLint Program<E0, EN...>::uniform(const GLchar *name) const {
			return glGetUniformLocation(m_id, name);
		}
		template<GLenum E0, GLenum... EN>
		Program<E0, EN...>::operator GLuint(void) const {
			return m_id;
		}
		template<GLenum E0, GLenum... EN>
		Shader& Program<E0, EN...>::operator[](unsigned index) {
			return shaders[index];
		}
		template<GLenum E0, GLenum... EN>
		Shader const& Program<E0, EN...>::operator[](unsigned index) const {
			return shaders[index];
		}
		template<GLenum E0, GLenum... EN>
		bool Program<E0, EN...>::build(void) const {
			for(auto i = 0; i < N; i++)
				if(!shaders[i].build()) return false;
			auto before = programIv(m_id, GL_ATTACHED_SHADERS);
			for(auto i = 0; i < N; i++) {
				glAttachShader(m_id, shaders[i]);
			}
			auto after = programIv(m_id, GL_ATTACHED_SHADERS);
			auto diff = after - before;
			if(diff != N) {
				for(auto i = 0; i < diff; i++)
					glDetachShader(m_id, shaders[i]);
				return false;
			}
			glLinkProgram(m_id);
			// TODO detach on link failure?
			if(!programAssertIv(m_id, GL_LINK_STATUS, GLint(GL_TRUE)))
				return false;
			glValidateProgram(m_id);
			return programAssertIv(m_id,
				GL_VALIDATE_STATUS, GLint(GL_TRUE));
		}
		template<GLenum E0, GLenum... EN>
		std::string Program<E0, EN...>::info(void) const {
			std::string out;
			for(auto i = 0; i < N; i++)
				out += shaders[i].info();
			return out + programInfo(m_id);
		}
		template<GLenum E0, GLenum... EN>
		bool Program<E0, EN...>::use(void) const {
			if(!programAssertIv(m_id, GL_LINK_STATUS, GLint(GL_TRUE)))
				return false;
			glUseProgram(m_id);
			return true;
		}
	}
}

#endif
