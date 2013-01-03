#include "vertexbufferfactory.hpp"

#include <fstream>
#include <vector>
#include <unordered_map>
#include <utility>

#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>

#include "src/modelloader/objparser.yy.hpp"
#include "objlexer.hpp"

#include "util/hash.hpp"

using namespace modelloader;

VertexBufferFactory VertexBufferFactory::instance;

const VertexBuffer& VertexBufferFactory::operator[](std::string objName) {
	//Check for VBO in flyweight pool
	if(vbopool.find(objName) != vbopool.end()) {
		return vbopool.at(objName);
	}

	//VBO not in flyweight pool: Load and buffer obj-file to VBO
	std::ifstream objFile("./models/" + objName + ".obj");

	if(!objFile.is_open()) {
		//TODO To logging
		std::cerr << "Could not find model " << objName << std::endl;
		assert(objFile.is_open());
	}

	modelloader::ObjLexer lexer(&objFile);

	std::vector<glm::vec3> verticesData;
	std::vector<unsigned int> indicesData;
	std::tuple<glm::vec3, glm::vec3> extremes;

	modelloader::ObjParser parser(verticesData, indicesData, extremes, lexer);
	parser.parse();

	std::vector<glm::vec3> vertexNormalsData = calculateVertexNormals(indicesData, verticesData);

	//Allocate room for a VBO and IBO
	GLuint bos[3];
	glGenBuffers(3, bos);

	glBindBuffer(GL_ARRAY_BUFFER, bos[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bos[1]);

	//Buffer vertices to VBO
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(glm::vec3), &(verticesData.front()), GL_STATIC_DRAW);

	//Buffer indices to IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(unsigned int), &(indicesData.front()), GL_STATIC_DRAW);

	//Buffer normals to NBO
	glBindBuffer(GL_ARRAY_BUFFER, bos[2]);
	glBufferData(GL_ARRAY_BUFFER, vertexNormalsData.size()*sizeof(glm::vec3), &(vertexNormalsData.front()), GL_STATIC_DRAW);

	vbopool.emplace(std::make_pair(std::string(objName), std::move(VertexBuffer{bos[0], bos[1], bos[2], indicesData.size(), extremes})));

	return vbopool.at(objName);
}

std::vector<glm::vec3> VertexBufferFactory::calculateVertexNormals(const std::vector<unsigned int>& indicesData, const std::vector<glm::vec3>& verticesData) {
	std::unordered_map<glm::vec3, glm::vec3> vertexNormalsMap;
	
	for(auto it = indicesData.cbegin(); it != indicesData.cend(); ++it) {
		const glm::vec3& p1 = verticesData.at(*it);
		const glm::vec3& p2 = verticesData.at(*++it);
		const glm::vec3& p3 = verticesData.at(*++it);
		
		glm::vec3 faceNormal = glm::triangleNormal(p1, p2, p3);
		
		vertexNormalsMap[p1] += faceNormal;
		vertexNormalsMap[p2] += faceNormal;
		vertexNormalsMap[p3] += faceNormal;
	}
	
	std::vector<glm::vec3> vertexNormals;
	
	std::for_each(indicesData.begin(), indicesData.end(), [&](const unsigned int& idx){
		const glm::vec3& vertex = verticesData.at(idx);
		const glm::vec3& vertexNormal = vertexNormalsMap.at(vertex);
		
		vertexNormals.push_back(vertexNormal);
	});
	
	return std::move(vertexNormals);
}
