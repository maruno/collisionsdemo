#include "render/shaderfactory.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

void render::ShaderFactory::checkShaderError(GLuint shader) {
	GLint succeeded;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &succeeded);

	if(succeeded == false) {
		GLint maxLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		char infoLog[maxLength];
		glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog);

		//TODO Omzetten naar logging!
		std::cerr << "Shader compile error:" << std::endl << infoLog << std::endl;

		assert(succeeded);
	}
}

const GLuint render::ShaderFactory::getShader(const std::string shaderName, GLenum shaderType) {
	//Check for shader in flyweight pool
	if(shaders.find(shaderName) != shaders.end()) {
		return shaders[shaderName];
	}
	
	//Shader not in flyweight pool: load and compile shader
	std::ifstream shaderSourceFile(std::string("./shaders/" + shaderName + ".glsl").c_str());
	
	if(!shaderSourceFile.is_open()) {
		//TODO To logging
		std::cerr << "Could not find shader " << shaderName << std::endl;
		assert(shaderSourceFile.is_open());
	}
	
	std::stringstream shaderSourceStream;
	
	while(shaderSourceFile.good()) {
		std::string line;
		std::getline(shaderSourceFile, line);

		shaderSourceStream << line;
	}

	GLuint shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, (const GLchar**) shaderSourceStream.str().c_str(), 0);
	glCompileShader(shader);
	checkShaderError(shader);
	
	shaders.insert(std::pair<std::string, GLuint>(shaderName, shader));
	
	return shader;
}
