#ifndef SHADERPIPELINE_HPP
#define SHADERPIPELINE_HPP

#include <string>
#include <cassert>

#include "glload/gl_3_2.h"

namespace render {
	class ShaderPipeLine {
		private:
			std::string vertexShaderName, geometryShaderName, fragmentShaderName;
			GLuint shaderProgram;

			void checkLinkError(const GLuint shader) const;
		public:
			ShaderPipeLine(const std::string myVertexShaderName, const std::string myFragmentShaderName);

			void setVertexShader(std::string myVertexShaderSource);
			void setGeometryShader(std::string myGeometryShaderSource);
			void setFragmentShader(std::string myFragmentShaderSource);

			const GLuint getShaderProgram() const;

			void linkPipeLine();
	};

	const GLuint ShaderPipeLine::getShaderProgram() const {
		return shaderProgram;
	}

	inline void ShaderPipeLine::setVertexShader(std::string myVertexShaderSource) {
		assert(myVertexShaderSource != "");

		vertexShaderName = myVertexShaderSource;
	}

	inline void ShaderPipeLine::setGeometryShader(std::string myGeometryShaderSource) {
		assert(myGeometryShaderSource != "");

		geometryShaderName = myGeometryShaderSource;
	}

	inline void ShaderPipeLine::setFragmentShader(std::string myFragmentShaderSource) {
		assert(myFragmentShaderSource != "");

		fragmentShaderName = myFragmentShaderSource;
	}
}
#endif // SHADERPIPELINE_HPP
