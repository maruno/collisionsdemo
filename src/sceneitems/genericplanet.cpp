#include "sceneitems/genericplanet.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "modelloader/vertexbufferfactory.hpp"

#include "render/shaderpipeline.hpp"
#include "render/matrixuniform.hpp"
#include "render/colourinformationuniform.hpp"
#include "render/lightmanager.hpp"
#include "scene/perspectivecamera.hpp"

using namespace sceneitems;

GenericPlanet::GenericPlanet(glm::vec3 initialLocation, unsigned int mySize)
	: sceneitems::Planet(initialLocation, mySize, 1.0f) {
	//Set colour to blue
	glGenBuffers(1, &colourUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, colourUBO);
	render::ColourInformationUniform colourInfo(glm::vec3(0.0f, 0.0f, 255.0f), 2);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(render::ColourInformationUniform), &colourInfo, GL_STATIC_DRAW);

	//Create and link shader-pipeline
	render::ShaderPipeLine shaderPipe("Vertex-Transform", "Fragment-Phong");
	shaderPipe.addShaderAttribute("vertex");
	shaderPipe.addShaderAttribute("normal");
	shaderPipe.setShaderUniform("matrixUni");
	shaderPipe.setShaderUniform("colourInformationUni");
	shaderPipe.setShaderUniform("lightsUni");
	shaderPipe.setShaderUniform("cameraUni");

	shaderPipe.linkPipeLine();
	shaderProgram = shaderPipe.getShaderProgram();
}

void GenericPlanet::render(glm::mat4& parentMatrix) const {
	//Update the MVP-matrix and buffer this to the UBO
	matrixMutex.lock();
	glm::mat4 mVMatrix = parentMatrix * modelMatrix;
	matrixMutex.unlock();

	render::MatrixUniform matrixUni(mVMatrix);

	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(render::MatrixUniform), &matrixUni, GL_DYNAMIC_DRAW);

	//Actual rendering calls
	vBuffers.bindBuffers();
	glUseProgram(shaderProgram);

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, matrixUBO);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, colourUBO);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, render::LightManager::getInstance().getUBO());
	glBindBufferBase(GL_UNIFORM_BUFFER, 4, scene::PerspectiveCamera::getInstance().getUBO());

	glDrawElements(GL_TRIANGLES, vBuffers.getNumIndices(), GL_UNSIGNED_INT, 0);
}
