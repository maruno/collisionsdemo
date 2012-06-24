#include "modelloader/vertexbufferfactory.hpp"

#include <fstream>
#include <vector>

#include <glm/glm.hpp>

#include "src/modelloader/objparser.yy.hpp"
#include "modelloader/objlexer.hpp"

using namespace modelloader;

VertexBufferFactory VertexBufferFactory::instance;

const VertexBuffer& VertexBufferFactory::operator[](std::string objName) {
	//Check for VBO in flyweight pool
	if(vbopool.find(objName) != vbopool.end()) {
		return vbopool.at(objName);
	}

	//VBO not in flyweight pool: Load and buffer obj-file to VBO
	std::ifstream inputFile("./models/" + objName + ".obj");
	modelloader::ObjLexer lexer(&inputFile);

	std::vector<float> verticesData;
	std::vector<unsigned int> indicesData;
	std::tuple<glm::vec3, glm::vec3> extremes;

	modelloader::ObjParser parser(verticesData, indicesData, extremes, lexer);
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

	VertexBuffer vertexBuffer(bos[0], bos[1], indicesData.size());

	vbopool.insert(std::pair<std::string, VertexBuffer>(objName, vertexBuffer));

	return vbopool.at(objName);
}
