#include "render/vertexbufferfactory.hpp"

#include "objectloader/objLoader.h"

using namespace render;

VertexBufferFactory VertexBufferFactory::instance;

const std::tuple<const GLuint, const GLuint> VertexBufferFactory::operator[] (std::string objName) {
	//Check for VBO in flyweight pool
	if(vbopool.find(objName) != vbopool.end()) {
		return vbopool[objName];
	}
	
	//VBO not in flyweight pool: Load and buffer obj-file to VBO
	objLoader loader;
	
	loader.load(const_cast<char*>(std::string("./models/" + objName + ".obj").c_str()));
	
	//Allocate room for a VBO and IBO
	GLuint bos[2];
	glGenBuffers(2, bos);
	
	glBindBuffer(GL_ARRAY_BUFFER, bos[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bos[1]);
	
	//Buffer vertices to VBO
	glBufferData(GL_ARRAY_BUFFER, loader.vertexCount * sizeof(obj_vector), *loader.vertexList, GL_STATIC_DRAW);
	
	//Create vertex indices array
	GLuint vertexindices[loader.faceCount * 3];
	
	for(int i = 0; i < loader.faceCount; i++) {
		obj_face face = *(loader.faceList)[i];
		
		vertexindices[i*3] = face.vertex_index[0];
		vertexindices[i*3+1] = face.vertex_index[1];
		vertexindices[i*3+2] = face.vertex_index[2];
	}
	
	//Buffer indices to IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, loader.faceCount * 3 * sizeof(GLuint), vertexindices, GL_STATIC_DRAW);
	
	std::tuple<GLuint, GLuint> vertexBuffer(bos[0], bos[1]);
	
	vbopool.insert(std::pair<std::string, std::tuple<GLuint, GLuint>>(objName, vertexBuffer));

	return vertexBuffer;
}