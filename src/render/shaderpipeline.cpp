#include "render/shaderpipeline.hpp"

#include <iostream>

#include "render/shaderfactory.hpp"

render::ShaderPipeLine::ShaderPipeLine(const std::string myVertexShaderName, const std::string myFragmentShaderName) {
	setVertexShader(myVertexShaderName);
	setFragmentShader(myFragmentShaderName);

	shaderProgram = glCreateProgram();
}

void render::ShaderPipeLine::checkLinkError(const GLuint shader) const {
	GLint succeeded;

	glGetProgramiv(shader, GL_LINK_STATUS, &succeeded);

	if(succeeded == false) {

		GLint maxLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		char infoLog[maxLength];
		glGetProgramInfoLog(shader, maxLength, &maxLength, infoLog);

		//TODO Omzetten naar logging!
		std::cerr << "Shader link error:" << std::endl << infoLog << std::endl;

		assert(succeeded);
	}
}

void render::ShaderPipeLine::linkPipeLine() {
	glAttachShader(shaderProgram, ShaderFactory::getShader(vertexShaderName, GL_VERTEX_SHADER));

	if(geometryShaderName != "") {
		glAttachShader(shaderProgram, ShaderFactory::getShader(geometryShaderName, GL_GEOMETRY_SHADER));
	}

	glAttachShader(shaderProgram, ShaderFactory::getShader(fragmentShaderName, GL_FRAGMENT_SHADER));

	//Attribute bindings
	int numAttrib = 0;
	for(auto it = attributes.begin(); it != attributes.end(); ++it) {
		glBindAttribLocation(shaderProgram, numAttrib, (*it).c_str());
		++numAttrib;
	}

	glLinkProgram(shaderProgram);
	checkLinkError(shaderProgram);

	//UBO bindings
	unsigned int numUBOs = 1;
	for(auto it = uniformBindings.begin(); it!= uniformBindings.end(); ++it) {
		GLuint uniformBlockIdx = glGetUniformBlockIndex(shaderProgram, it->first.c_str());

		glBindBufferBase(GL_UNIFORM_BUFFER, numUBOs, it->second);
		glUniformBlockBinding(shaderProgram, uniformBlockIdx, numUBOs);

		++numUBOs;
	}
}
