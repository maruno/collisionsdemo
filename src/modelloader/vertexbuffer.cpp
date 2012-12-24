#include "vertexbuffer.hpp"

using namespace modelloader;

VertexBuffer::VertexBuffer(GLuint myVBO, GLuint myIBO, GLuint myNBO, unsigned int myNumIndices)
: vBO(myVBO), iBO(myIBO), nBO(myNBO), numIndices(myNumIndices) {
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