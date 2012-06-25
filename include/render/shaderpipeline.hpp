#ifndef SHADERPIPELINE_HPP
#define SHADERPIPELINE_HPP

#include <string>
#include <cassert>
#include <list>
#include <queue>

#include "glload/gl_3_2.h"

namespace render {
	/**
	 * Simple util class for creating a shader pipeline. Can be used to create a shaderprogram.
	 *
	 * @author Michel Bouwmans
	 */
	class ShaderPipeLine {
		private:
			std::string vertexShaderName, geometryShaderName, fragmentShaderName;
			GLuint shaderProgram;
			std::list<std::string> attributes;
			std::queue<std::string> uniformBindings;

			void checkLinkError(const GLuint shader) const;
		public:
			/**
			 * Constructor.
			 *
			 * @param myVertexShaderName Name of the vertex shader, equalling the filename without the glsl-extension.
			 * @param myFragmentShaderName Name of the fragment shader, equalling the filename without the glsl-extension.
			 */
			ShaderPipeLine(const std::string myVertexShaderName, const std::string myFragmentShaderName);

			/**
			 * Overwrite the previously set vertex shader.
			 *
			 * @param myVertexShaderName Name of the new vertex shader, equalling the filename without the glsl-extension.
			 */
			void setVertexShader(std::string myVertexShaderSource);

			/**
			 * Set the optional geometry shader.
			 *
			 * @param myGeometryShaderSource Name of the new geometry shader, equalling the filename without the glsl-extension.
			 */
			void setGeometryShader(std::string myGeometryShaderSource);

			/**
			 * Overwrite the previously set fragment shader.
			 *
			 * @param myVertexShaderName Name of the new fragment shader, equalling the filename without the glsl-extension.
			 */
			void setFragmentShader(std::string myFragmentShaderSource);

			/**
			 * Add a per-vertex attribute to the shader pipeline.
			 * Attributes should be added in the same order as their entries in the Vertex Array Object.
			 *
			 * @param attributeName Name of the per-vertex attribute.
			 */
			void addShaderAttribute(std::string attributeName);

			/**
			 * Sets a uniform-binding for the shader pipeline.
			 *
			 * @param uniformName Name of the uniform block.
			 */
			void setShaderUniform(std::string uniformName);

			/**
			 * Get the shader program associated with this shader pipeline.
			 *
			 * @return Shader program
			 */
			const GLuint getShaderProgram() const;

			/**
			 * Link all elements of this shader pipeline to a ready-for-use shader program.
			 * This function fails on error.
			 */
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

	inline void ShaderPipeLine::setShaderUniform(std::string uniformName) {
		assert(uniformName != "");
		
		uniformBindings.push(uniformName);
	}
}
#endif // SHADERPIPELINE_HPP
