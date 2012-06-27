#include "sceneitems/planet.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "modelloader/vertexbufferfactory.hpp"

using namespace sceneitems;

GLuint Planet::vao = 0;

Planet::Planet(glm::vec3 initialLocation, unsigned int mySize, unsigned int density)
	: bSphere(size, initialLocation), scene::GravitationalObject(initialLocation, mySize* density * 10000, bSphere), size(mySize),
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
	
	matrixMutex.lock();
	
	modelMatrix = glm::translate(glm::mat4(1.0f), location);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(size));
	
	matrixMutex.unlock();
}
