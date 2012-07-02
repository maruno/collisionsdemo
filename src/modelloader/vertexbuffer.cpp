#include "modelloader/vertexbuffer.hpp"

using namespace modelloader;

VertexBuffer::VertexBuffer(GLuint myVBO, GLuint myIBO, GLuint myNBO, unsigned int myNumIndices)
	: vBO(myVBO), iBO(myIBO), nBO(myNBO), numIndices(myNumIndices) {
}
