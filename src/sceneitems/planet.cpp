#include "sceneitems/planet.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "modelloader/vertexbufferfactory.hpp"

using namespace sceneitems;

GLuint Planet::vao = 0;

Planet::Planet(glm::vec3 initialLocation, unsigned int mySize, unsigned int density)
	: scene::GravitationalObject(initialLocation, mySize* density * 10000), size(mySize),
	  vBuffers(modelloader::VertexBufferFactory::getInstance()["sphere"]) {

	if(vao == 0) {
		//Generate VAO for Planets
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vBuffers.getVBO());
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(0);
	}
}

void Planet::update() {
	scene::GravitationalObject::update();

	modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(size));
	modelMatrix = glm::translate(modelMatrix, location);
}