#include <cassert>

#include <unistd.h>

#include "glload/gl_3_2.h"
#include "glload/gll.hpp"
#include <GL/glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/shaderpipeline.hpp"
#include "heightmap/heightmapgenerator.hpp"
#include "scene/world.hpp"

int main(int argc, char** argv) {
	glfwInit();

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 8);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width = 1024;
	int height = 768;

	glfwOpenWindow(width, height, 8, 8, 8, 8, 24, 24, GLFW_WINDOW);
	glfwSetWindowTitle("OpenGL 3.2 Core profile test");

	//Load OpenGL functions
	if(glload::LoadFunctions() == glload::LS_LOAD_FAILED) {
		return 1;
	}
	
	//Define world
	scene::World::rescale(width, height);

	//Set clear colour to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Create VAO and VBO
	GLuint vaos[1];
	glGenVertexArrays(1, vaos);
	glBindVertexArray(vaos[0]);

	GLuint vbos[1];
	glGenBuffers(1, vbos);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);

	//Generate heightmap
	HeightmapGenerator* hmgen = new HeightmapGenerator(65, 0, 64);
	hmgen->fillMap();
	hmgen->convertMap();

	//Buffer heightmap-data to VBO
	unsigned int vertexCount = hmgen->getVertexCount();
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(HMVertex), hmgen->getVertices(), GL_STATIC_DRAW);

	//Set vertex attribute index 0 to current VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Matrices
	glm::mat4 model = glm::scale<float>(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));

	glm::mat4 mVPMatrix = scene::World::getMatrix() * model;

	//Create UBO
	GLuint ubos[1];
	glGenBuffers(1, ubos);
	glBindBuffer(GL_UNIFORM_BUFFER, ubos[0]);

	//TODO Interactivity
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), glm::value_ptr(mVPMatrix), GL_DYNAMIC_DRAW);

	//Create shader pipeline
	render::ShaderPipeLine shaderpipe("Vertex-Pass Y", "Fragment-Colour Height");

	shaderpipe.addShaderAttribute("vertex");

	shaderpipe.linkPipeLine();
	GLuint shaderProgram = shaderpipe.getShaderProgram();

	//UBO bindings... TODO OOP
	GLuint hmMatricesUniformBlockIdx = glGetUniformBlockIndex(shaderProgram, "hmMatrices");
	glUniformBlockBinding(shaderProgram, hmMatricesUniformBlockIdx, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubos[0]);

	glUseProgram(shaderProgram);

	//Main render loop
	while(true) {
		usleep(500000);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);

		glfwSwapBuffers();
	}

	glfwTerminate();
}