#ifndef SHADERFACTORY_HPP
#define SHADERFACTORY_HPP

#include <map>
#include <string>

#include "glload/gl_3_2.h"

namespace render {
	/**
	 * Flyweight factory class for shaders. Administrates and creates shaders.
	 *
	 * @author Michel Bouwmans
	 */
	class ShaderFactory {
	 private:
		static std::map<const std::string, const GLuint> shaders;
		static void checkShaderError(GLuint shader);
	 public:
		/**
		 * Returns the requested shader. Shader is returned from flyweight pool if available.
		 * This method tries to fetch and compile a shader if not in the pool and fails on error.
		 *
		 * @param shaderType Type of the shader as known to OpenGL
		 * @param shaderName Name of the shader to return, equalling the filename without the glsl-extension
		 */
		static const GLuint getShader(const std::string shaderName, GLenum shaderType);
	};
}

#endif // SHADERFACTORY_HPP
