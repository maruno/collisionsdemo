#include "render/shaderfactory.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <cstring>

std::map<const std::string, const GLuint> render::ShaderFactory::shaders;

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

		shaderSourceStream << line << std::endl;
	}

	//We need to copy the C-string because of the volatile characteristics of std::string.c_str()
	char* shaderSourceCString = new char[shaderSourceStream.str().size()+1];
	std::strcpy(shaderSourceCString, shaderSourceStream.str().c_str());

	GLuint shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, (const GLchar**) &shaderSourceCString, 0);
	glCompileShader(shader);
	checkShaderError(shader);
	
	delete[] shaderSourceCString;
	
	shaders.insert(std::pair<std::string, GLuint>(shaderName, shader));

	return shader;
}
