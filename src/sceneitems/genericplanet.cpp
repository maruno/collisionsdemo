#include "sceneitems/genericplanet.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "modelloader/vertexbufferfactory.hpp"

#include "render/shaderpipeline.hpp"

using namespace sceneitems;

GLuint GenericPlanet::vao = 0;

GenericPlanet::GenericPlanet(glm::vec3 initialLocation, unsigned int mySize, unsigned int density)
	: scene::GravitationalObject(initialLocation, mySize * density * 10000), size(mySize),
	  vBuffers(modelloader::VertexBufferFactory::getInstance()["sphere"]) {

	if(vao == 0) {
		//Generate VAO for Planets
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vBuffers.getVBO());
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(0);
	}

	//Set colour to blue
	glBindBuffer(GL_UNIFORM_BUFFER, colourUBO);
	glm::vec3 colour(0.0f, 0.0f, 1.0f);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), glm::value_ptr(colour), GL_STATIC_DRAW);
	
	//Create and link shader-pipeline
	render::ShaderPipeLine shaderPipe("Vertex-transform", "Fragment-Colour-Bucket");
	shaderPipe.addShaderAttribute("vertex");
	shaderPipe.setShaderUniform("mVPMatrix", matrixUBO);
	shaderPipe.setShaderUniform("colour", colourUBO);
	
	shaderPipe.linkPipeLine();
	shaderProgram = shaderPipe.getShaderProgram();
}

void GenericPlanet::update() {
	scene::GravitationalObject::update();
	
	modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(size));
	modelMatrix = glm::translate(modelMatrix, location);
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
