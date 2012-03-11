#include "render/shaderpipeline.hpp"

#include <iostream>

render::ShaderPipeLine::ShaderPipeLine(std::string myVertexShaderSource, std::string myFragmentShaderSource) {
	setVertexShader(myVertexShaderSource);
	setFragmentShader(myFragmentShaderSource);

	shaderProgram = glCreateProgram();
}

void render::ShaderPipeLine::checkShaderError(GLuint shader, render::ShaderPipeLine::ShaderErrorType errorType) {
	GLboolean succeeded;

	if(errorType == compile) {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &succeeded);
		}
	else {
			glGetProgramiv(shader, GL_LINK_STATUS, &succeeded);
		}

	if(succeeded == false) {
			unsigned int maxLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			/* The maxLength includes the NULL character */
			char infoLog[] = new char[maxLength];

			std::string errorName;
			if(errorType == compile) {
					glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog);

					errorName = "compile";
				}
			else {
					glGetProgramInfoLog(shader, maxLength, &maxLength, infoLog);

					errorName = "link";
				}

			//TODO Omzetten naar logging!
			std::cerr << "Shader " << errorName << " error:" << std::endl << infoLog << std::endl;

			/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
			/* In this simple program, we'll just leave */
			delete[] infoLog;

			assert(succeeded);
		}
}

void render::ShaderPipeLine::compilePipeLine() {
	GLuint vertexShader, fragmentShader;

	//Compile vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, vertexShaderSource.c_str(), 0);
	glCompileShader(vertexShader);
	checkShaderError(vertexShader, compile);

	//Compile geometry shader if available
	if(geometryShaderSource != "") {
			GLuint geometryShader;
			geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

			glShaderSource(geometryShader, 1, geometryShaderSource.c_str(), 0);
			glCompileShader(geometryShader);
			checkShaderError(geometryShader, compile);
		}

	//Compile fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, vertexShaderSource.c_str(), 0);
	glCompileShader(fragmentShader);
	checkShaderError(vertexShader, compile);

	//Link shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//TODO Attributes!!! Shader class!!!

	glLinkProgram(shaderProgram);
	checkShaderError();
}
