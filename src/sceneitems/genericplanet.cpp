#include "sceneitems/genericplanet.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "modelloader/vertexbufferfactory.hpp"

#include "render/shaderpipeline.hpp"

using namespace sceneitems;

GenericPlanet::GenericPlanet(glm::vec3 initialLocation, unsigned int mySize)
	: sceneitems::Planet(initialLocation, mySize, 1.0f) {
	//Set colour to blue
	glGenBuffers(1, &colourUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, colourUBO);
	glm::vec3 colour(0.0f, 0.0f, 1.0f);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), glm::value_ptr(colour), GL_STATIC_DRAW);

	//Create and link shader-pipeline
	render::ShaderPipeLine shaderPipe("Vertex-Transform", "Fragment-Colour");
	shaderPipe.addShaderAttribute("vertex");
	shaderPipe.setShaderUniform("mVPMatrixUni", matrixUBO);
	shaderPipe.setShaderUniform("colourUni", colourUBO);

	shaderPipe.linkPipeLine();
	shaderProgram = shaderPipe.getShaderProgram();
}

void GenericPlanet::render(glm::mat4& parentMatrix) const {
	//Update the MVP-matrix and buffer this to the UBO
	glm::mat4 mVPMatrix = parentMatrix * modelMatrix;

	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), glm::value_ptr(mVPMatrix), GL_DYNAMIC_DRAW);

	//Actual rendering calls
	glBindVertexArray(vao);
	glUseProgram(shaderProgram);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vBuffers.getIBO());
	glDrawElements(GL_TRIANGLES, vBuffers.getNumIndices(), GL_UNSIGNED_INT, 0);
}
