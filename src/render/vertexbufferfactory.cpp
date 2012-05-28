#include "render/vertexbufferfactory.hpp"

#include <fstream>
#include <vector>

#include "src/modelloader/objparser.yy.hpp"
#include "modelloader/objlexer.hpp"

using namespace render;

VertexBufferFactory VertexBufferFactory::instance;

const std::tuple<const GLuint, const GLuint> VertexBufferFactory::operator[] (std::string objName) {
	//Check for VBO in flyweight pool
	if(vbopool.find(objName) != vbopool.end()) {
		return vbopool[objName];
	}
	
	//VBO not in flyweight pool: Load and buffer obj-file to VBO
	std::ifstream inputFile("./models/" + objName + ".obj");
	modelloader::ObjLexer lexer(&inputFile);
	
	std::vector<float> verticesData;
	std::vector<unsigned int> indicesData;
	
	modelloader::ObjParser parser(verticesData, indicesData, lexer);
	parser.parse();
	
	//Allocate room for a VBO and IBO
	GLuint bos[2];
	glGenBuffers(2, bos);
	
	glBindBuffer(GL_ARRAY_BUFFER, bos[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bos[1]);
	
	//Buffer vertices to VBO
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(float), &(verticesData.front()), GL_STATIC_DRAW);
	
	//Buffer indices to IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(unsigned int), &(indicesData.front()), GL_STATIC_DRAW);
	
	std::tuple<GLuint, GLuint> vertexBuffer(bos[0], bos[1]);
	
	vbopool.insert(std::pair<std::string, std::tuple<GLuint, GLuint>>(objName, vertexBuffer));

	return vertexBuffer;
}