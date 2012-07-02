#include "modelloader/vertexbufferfactory.hpp"

#include <fstream>
#include <vector>
#include <map>
#include <utility>

#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>

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

	vertex_facenormals vertexNormalsPrep;
	calculateFaceNormals(vertexNormalsPrep, verticesData, indicesData);

	std::vector<glm::vec3> vertexNormalsData;
	calculateVertexNormals(vertexNormalsData, vertexNormalsPrep);

	//Allocate room for a VBO and IBO
	GLuint bos[3];
	glGenBuffers(3, bos);

	glBindBuffer(GL_ARRAY_BUFFER, bos[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bos[1]);

	//Buffer vertices to VBO
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(float), &(verticesData.front()), GL_STATIC_DRAW);

	//Buffer indices to IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(unsigned int), &(indicesData.front()), GL_STATIC_DRAW);

	//Buffer normals to NBO
	glBindBuffer(GL_ARRAY_BUFFER, bos[2]);
	glBufferData(GL_ARRAY_BUFFER, vertexNormalsData.size()*sizeof(glm::vec3), &(vertexNormalsData.front()), GL_STATIC_DRAW);

	VertexBuffer vertexBuffer(bos[0], bos[1], bos[2], indicesData.size());

	vbopool.insert(std::pair<std::string, VertexBuffer>(objName, vertexBuffer));

	return vbopool.at(objName);
}

void VertexBufferFactory::calculateFaceNormals(vertex_facenormals& vertexNormalsPrep,
												const std::vector<float>& verticesData,
												const std::vector<unsigned int>& indicesData) {
	for(int index = 0; index < indicesData.size(); index += 3) {
		unsigned int point1ID = indicesData[index];
		unsigned int point2ID = indicesData[index+1];
		unsigned int point3ID = indicesData[index+2];

		glm::vec3 point1;
		glm::vec3 point2;
		glm::vec3 point3;

		point1.x = verticesData[point1ID];
		point1.y = verticesData[point1ID+1];
		point1.z = verticesData[point1ID+2];

		point2.x = verticesData[point2ID];
		point2.y = verticesData[point2ID+1];
		point2.z = verticesData[point2ID+2];

		point3.x = verticesData[point3ID];
		point3.y = verticesData[point3ID+1];
		point3.z = verticesData[point3ID+2];

		glm::vec3 faceNormal = glm::triangleNormal(point1, point2, point3);

		auto itr = vertexNormalsPrep.find(point1);
		if(itr == vertexNormalsPrep.end()) {
			face_normals faceNormals;
			faceNormals.push_back(faceNormal);
			vertexNormalsPrep.insert(std::make_pair(point1, faceNormals));
		} else {
			itr->second.push_back(faceNormal);
		}

		itr = vertexNormalsPrep.find(point2);
		if(itr == vertexNormalsPrep.end()) {
			face_normals faceNormals;
			faceNormals.push_back(faceNormal);
			vertexNormalsPrep.insert(std::make_pair(point2, faceNormals));
		} else {
			itr->second.push_back(faceNormal);
		}

		itr = vertexNormalsPrep.find(point3);
		if(itr == vertexNormalsPrep.end()) {
			face_normals faceNormals;
			faceNormals.push_back(faceNormal);
			vertexNormalsPrep.insert(std::make_pair(point3, faceNormals));
		} else {
			itr->second.push_back(faceNormal);
		}
	}
}

void VertexBufferFactory::calculateVertexNormals(std::vector<glm::vec3>& vertexNormalsData,
													const vertex_facenormals& vertexNormalsPrep) {
	std::for_each(vertexNormalsPrep.cbegin(), vertexNormalsPrep.cend(), [&](const std::pair<glm::vec3, face_normals>& kvPair) {
		face_normals faceNormals = kvPair.second;

		glm::vec3 vertexNormal;

		face_normals::iterator faceItr;
		for(faceItr = faceNormals.begin(); faceItr != faceNormals.end(); ++faceItr) {
			vertexNormal += (*faceItr);
		}

		vertexNormalsData.push_back(vertexNormal);
	});
}
