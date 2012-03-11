#include <unistd.h>

#include "glload/gl_3_2.h"
#include "glload/gll.hpp"

#include <GL/glfw.h>
#include <cassert>

#include "render/shaderpipeline.hpp"

int main(int argc, char **argv) {
	glfwInit();
	
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 8);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	glfwOpenWindow(1024, 768, 8, 8, 8, 8, 24, 24, GLFW_WINDOW);
	glfwSetWindowTitle("OpenGL 3.2 Core profile test");
	
	//Load OpenGL functions
	if(glload::LoadFunctions() == glload::LS_LOAD_FAILED) {
		return 1;
	}
	
	//Set clear colour to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	typedef struct {
		GLfloat x, y, z;
	} Vertex;
	
	Vertex vertices[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.5f, 1.0f, -0.5f,
		
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		
		0.5f, 1.0f, -0.5f,
		0.0f, 0.0f, 1.0f,
		
		0.5f, 1.0f, -0.5f,
		0.0f, 0.0f, 0.0f
	};
	
	GLuint vaos[1];
	glGenVertexArrays(1, vaos);
	glBindVertexArray(vaos[0]);
	
	GLuint vbos[1];
	glGenBuffers(1, vbos);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	
	glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glEnableVertexAttribArray(0);
	
	//TODO glBindAttribLocation(shaderprogram, 0, "vertex");
	
	//TODO Naar vertex object class!!!
	render::ShaderPipeLine shaderpipe("SOURCE LADEN!", "SOURCE LADEN!");
	shaderpipe.compilePipeLine();
	
	GLuint shaderProgram = shaderpipe.getShaderProgram();
	
	
	while(true) {
		usleep(500000);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		glDrawArrays(GL_TRIANGLE_FAN, 0, 9);
		
		GLenum err = glGetError();
		
		switch(err) {
			case GL_INVALID_ENUM:
				std::cout << "Error: GL_INVALID_ENUM" << std::endl;
				break;
			case GL_INVALID_VALUE:
				std::cout << "Error: GL_INVALID_VALUE" << std::endl;
				break;
			case GL_INVALID_OPERATION:
				std::cout << "Error: GL_INVALID_OPERATION" << std::endl;
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				std::cout << "Error: GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
				break;
		}
	}
	
	glfwTerminate();
}
