#include <cassert>

#include <unistd.h>

#include "glload/gl_3_2.h"
#include "glload/gll.hpp"
#include <GL/glfw.h>

#include "render/shaderpipeline.hpp"
#include "heightmap/heightmapgenerator.hpp"

int main(int argc, char** argv) {
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

	glViewport(0, 0, 1024, 768);

	//Set clear colour to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLuint vaos[1];
	glGenVertexArrays(1, vaos);
	glBindVertexArray(vaos[0]);

	GLuint vbos[1];
	glGenBuffers(1, vbos);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);

	HeightmapGenerator* hmgen = new HeightmapGenerator(1025, 0, 512);
	hmgen->fillMap();
	hmgen->convertMap();

	unsigned int vertexCount = hmgen->getVertexCount();

	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(HMVertex), hmgen->getVertices(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);

	render::ShaderPipeLine shaderpipe("Vertex-Pass Y", "Fragment-Colour Height");

	shaderpipe.addShaderAttribute("vertex");

	shaderpipe.linkPipeLine();
	GLuint shaderProgram = shaderpipe.getShaderProgram();

	glUseProgram(shaderProgram);

	while(true) {
		usleep(500000);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);

		glfwSwapBuffers();
	}

	glfwTerminate();
}
