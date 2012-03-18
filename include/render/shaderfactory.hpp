#ifndef SHADERFACTORY_HPP
#define SHADERFACTORY_HPP

#include <map>
#include <string>

#include "glload/gl_3_2.h"

namespace render {
	class ShaderFactory {
		private:
			static std::map<const std::string, const GLuint> shaders;
			static void checkShaderError(GLuint shader);
		public:
			static const GLuint getShader(const std::string shaderName, GLenum shaderType);
	};
}

#endif // SHADERFACTORY_HPP
