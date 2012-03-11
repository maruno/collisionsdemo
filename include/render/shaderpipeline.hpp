#ifndef SHADERPIPELINE_HPP
#define SHADERPIPELINE_HPP

#include <string>
#include <cassert>

#include "glload/gl_3_2.h"

namespace render {
	class ShaderPipeLine {
		private:
			enum ShaderErrorType {
				compile, link
			};

			std::string vertexShaderSource, geometryShaderSource, fragmentShaderSource;
			GLuint shaderProgram;

			void checkShaderError(GLuint shader, ShaderErrorType errorType);
		public:
			ShaderPipeLine(std::string myVertexShaderSource, std::string myFragmentShaderSource);

			void setVertexShader(std::string myVertexShaderSource);
			void setGeometryShader(std::string myGeometryShaderSource);
			void setFragmentShader(std::string myFragmentShaderSource);

			GLuint getShaderProgram();

			void compilePipeLine();
	};

	inline GLuint ShaderPipeLine::getShaderProgram() {
		return shaderProgram;
	}

	inline void ShaderPipeLine::setVertexShader(std::string myVertexShaderSource) {
		assert(myVertexShaderSource != "");

		vertexShaderSource = myVertexShaderSource;
	}

	inline void ShaderPipeLine::setGeometryShader(std::string myGeometryShaderSource) {
		assert(myGeometryShaderSource != "");

		geometryShaderSource = myGeometryShaderSource;
	}

	inline void ShaderPipeLine::setFragmentShader(std::string myFragmentShaderSource) {
		assert(myFragmentShaderSource != "");

		fragmentShaderSource = myFragmentShaderSource;
	}
}
#endif // SHADERPIPELINE_HPP
