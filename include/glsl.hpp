/*! @file glsl.hpp
 *  @brief Declarations of shader sources and programs */

#ifndef GLSL_HPP
#define GLSL_HPP

// #include <SDL2/SDL_video.h>

namespace View {
	using namespace Abstract;

	namespace Shaders {

		/** @brief RAII shader program with at least one member
		 * @tparam E0 Type of the first shader member
		 * @tparam EN Type of the remaining shader members
		 */
		template<GLenum E0, GLenum... EN> struct Program;

		/** @brief Sets and returns the value for the given shader.
		 * @param id The SDL-issued shader id
		 * @param k The key for the given value
		 * @param pdest Pointer to the value to assign to (optional)
		 * @return The resulting value, potentially referenced by pdest */
		GLint shaderIv(GLuint id, GLenum k, GLint *pdest = 0);
		/** @brief Sets and returns the value for the given program.
		 * @param id The SDL-issued shader program id
		 * @param k The key for the given value
		 * @param pdest Pointer to the value to assign to (optional)
		 * @return The resulting value, potentially referenced by pdest */
		GLint programIv(GLuint id, GLenum k, GLint *pdest = 0);

		/** @brief Sets and returns the deduced query result. */
		GLint queryIv(GLuint id, GLenum k, GLint *pdest = 0);
		/** @brief Compares the query result to the given target. */
		bool shaderAssertIv(GLuint id, GLenum key, GLint val);
		/** @brief Compares the query result to the given target. */
		bool programAssertIv(GLuint id, GLenum key, GLint val);
		/** @brief Compares the deduced query result to the given target. */
		bool queryAssertIv(GLuint id, GLenum key, GLint val);

		/** @brief Returns the log associated with the given shader. */
		std::string shaderInfo(GLuint id);
		/** @brief Returns the log associated with the given program. */
		std::string programInfo(GLuint id);
		/** @brief Returns the log associated with the deduced object. */
		std::string queryInfo(GLuint id);


		/** @brief RAII shader alloc/source/compile/dealloc */
		struct Shader {
		protected:
			const GLuint m_id;
		public:
			const bool m_isProgram = bool(glIsProgram(m_id)),
				m_isShader = !m_isProgram && glIsShader(m_id);
			operator GLuint(void) const;
			bool build(void) const;
			std::string info(void) const;
			Shader(void);
			Shader(GLenum E);
			Shader(GLenum E, std::string const& src);
			//Shader(GLuint id); //-- 'Future' until est. mgmt policy
			virtual ~Shader(void);
		};
		/* @brief RAII collection of shader sources as a shader program.
		 * @tparam E0 Type of the first shader (there must be at least one)
		 * @tparam EN Type of remaining shaders */
		template<GLenum E0, GLenum... EN>
		struct Program: Shader {
			static constexpr auto N = sizeof...(EN)+1;
			Shader shaders[N] = {{E0}, {EN}...};
			const GLenum types[N] = {E0, EN...};
			GLint uniform(const GLchar *name) const;
			operator GLuint(void) const;
			Shader& operator[](unsigned index);
			Shader const& operator[](unsigned index) const;
			bool build(void) const;
			std::string info(void) const;
			bool use(void) const;
			template<typename T0, typename... TN>
			Program(T0 const& t0, TN const&... tn):
				Shader(), shaders{Shader(E0, t0), Shader(EN, tn)...} {}
			// TODO detach shaders from destructor?
		};
	}
}

#include "glsl.tpp"

#endif
