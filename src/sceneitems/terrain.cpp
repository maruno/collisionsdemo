#include "sceneitems/terrain.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "heightmap/heightmapgenerator.hpp"
#include "render/shaderpipeline.hpp"

sceneitems::Terrain::Terrain(const glm::mat4 myParentMatrix, unsigned int size, unsigned int variance) : parentMatrix(myParentMatrix), rotation(STATIONARY) {
	//Create VAO and VBO
	glGenVertexArrays(1, vaos);
	glBindVertexArray(vaos[0]);

	glGenBuffers(1, vbos);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);

	//Generate heightmap
	HeightmapGenerator hmgen(size, 0, variance);
	hmgen.fillMap();
	hmgen.convertMap();

	//Buffer heightmap-data to VBO
	vertexCount = hmgen.getVertexCount();
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(HMVertex), hmgen.getVertices(), GL_STATIC_DRAW);

	//Set vertex attribute index 0 to current VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Create UBO's
	glGenBuffers(2, ubos);

	//Save max heightmap height
	unsigned int maxHeight = variance * 2;

	glBindBuffer(GL_UNIFORM_BUFFER, ubos[1]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(unsigned int), &maxHeight, GL_STATIC_DRAW);

	//Scale 0.1x
	modelMatrix = glm::scale<float>(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
	updateMatrix();

	//Create shader pipeline
	render::ShaderPipeLine shaderpipe("Vertex-Transform-Map Colour", "Fragment-Colour Height");
	shaderpipe.addShaderAttribute("vertex");
	shaderpipe.setShaderUniform("hmMatrices", ubos[0]);
	shaderpipe.setShaderUniform("hmHeight", ubos[1]);

	shaderpipe.linkPipeLine();
	shaderProgram = shaderpipe.getShaderProgram();
}

void sceneitems::Terrain::updateMatrix() {
	glBindBuffer(GL_UNIFORM_BUFFER, ubos[0]);

	//Rotation
	float degrees;
	switch(rotation) {
	case LEFT:
		degrees = 1.0f;
		break;
	case RIGHT:
		degrees = -1.0f;
		break;
	}

	if(rotation != STATIONARY) {
		modelMatrix = glm::rotate<float>(modelMatrix, degrees, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	//HACK Temporarily get a new parentMatrix from World till we have a proper scene graph
	parentMatrix = scene::PerspectiveCamera::getMatrix();

	glm::mat4 mVPMatrix = parentMatrix * modelMatrix;

	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), glm::value_ptr(mVPMatrix), GL_DYNAMIC_DRAW);
}

void sceneitems::Terrain::render() const {
	glBindVertexArray(vaos[0]);
	glUseProgram(shaderProgram);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);
}
