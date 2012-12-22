#include "shaderpipeline.hpp"

#include <iostream>

#include "shaderfactory.hpp"

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

		volatile char* infoLog = new char[maxLength];
		glGetProgramInfoLog(shader, maxLength, &maxLength, (char*) infoLog);

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
	while(!uniformBindings.empty()) {
		auto uniform = uniformBindings.front();
		
		GLuint uniformBlockIdx = glGetUniformBlockIndex(shaderProgram, uniform.c_str());
		glUniformBlockBinding(shaderProgram, uniformBlockIdx, numUBOs);
		
		uniformBindings.pop();
		++numUBOs;
	}
}
