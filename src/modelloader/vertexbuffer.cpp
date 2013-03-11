#include "vertexbuffer.hpp"

#include "collisiondetection/objectorientedboundingbox.hpp"

using namespace modelloader;

VertexBuffer::VertexBuffer(VertexBuffer&& other) : vBO{other.vBO}, iBO{other.iBO}, nBO{other.nBO}, vao(other.vao), numIndices{other.numIndices}, extremes{other.extremes},
bounds(std::move(other.bounds)){
}

VertexBuffer::VertexBuffer(GLuint myVBO, GLuint myIBO, GLuint myNBO, unsigned int myNumIndices, std::tuple<glm::vec3, glm::vec3> myExtremes)
: vBO(myVBO), iBO(myIBO), nBO(myNBO), numIndices(myNumIndices), extremes(myExtremes), bounds{new collisiondetection::ObjectOrientedBoundingBox(extremes)} {
	//Generate VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, nBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
}

void VertexBuffer::bindBuffers() const {
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBO);
}