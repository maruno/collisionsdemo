#include "modelloader/vertexbuffer.hpp"

using namespace modelloader;

VertexBuffer::VertexBuffer(GLuint myVBO, GLuint myIBO, unsigned int myNumIndices)
	: vBO(myVBO), iBO(myIBO), numIndices(myNumIndices) {
}
