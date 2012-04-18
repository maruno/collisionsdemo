#ifndef SHADERPIPELINE_HPP
#define SHADERPIPELINE_HPP

#include <string>
#include <cassert>
#include <list>
#include <map>

#include "glload/gl_3_2.h"

namespace render {
	class ShaderPipeLine {
		private:
			std::string vertexShaderName, geometryShaderName, fragmentShaderName;
			GLuint shaderProgram;
			std::list<std::string> attributes;
			std::map<std::string, GLuint> uniformBindings;

			void checkLinkError(const GLuint shader) const;
		public:
			ShaderPipeLine(const std::string myVertexShaderName, const std::string myFragmentShaderName);

			void setVertexShader(std::string myVertexShaderSource);
			void setGeometryShader(std::string myGeometryShaderSource);
			void setFragmentShader(std::string myFragmentShaderSource);
			
			void addShaderAttribute(std::string attributeName);
			void setShaderUniform(std::string uniformName, GLuint uboId);

			const GLuint getShaderProgram() const;

			void linkPipeLine();
	};

	inline const GLuint ShaderPipeLine::getShaderProgram() const {
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
	
	inline void ShaderPipeLine::addShaderAttribute(std::string attributeName) {
		assert(attributeName != "");
		
		attributes.push_back(attributeName);
	}
	
	inline void ShaderPipeLine::setShaderUniform(std::string uniformName, GLuint uboId) {
		assert(uniformName != "");
		
		uniformBindings.erase(uniformName);
		uniformBindings.insert(std::pair<std::string, GLuint>(uniformName, uboId));
	}
}
#endif // SHADERPIPELINE_HPP
