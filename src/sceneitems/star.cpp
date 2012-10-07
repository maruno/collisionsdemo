#include "sceneitems/star.hpp"

#include <mutex>

#include "render/shaderpipeline.hpp"
#include "render/matrixuniform.hpp"
#include "render/colourinformationuniform.hpp"
#include "render/sources.hpp"

using namespace sceneitems;

Star::Star(glm::vec3 initialLocation, unsigned int mySize)
	: sceneitems::Planet(initialLocation, mySize, 2.5f) {
	//Set colour to light yellow
	glGenBuffers(1, &colourUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, colourUBO);
	render::ColourInformationUniform colourInfo(glm::vec3(255.0f, 254.0f, 176.0f), 0);
	colourInfo.ambiance = glm::vec3(0.8f);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(render::ColourInformationUniform), &colourInfo, GL_STATIC_DRAW);

	//Create and link shader-pipeline
	render::ShaderPipeLine shaderPipe("Vertex-Transform", "Fragment-Phong");
	shaderPipe.addShaderAttribute("vertex");
	shaderPipe.addShaderAttribute("normal");
	shaderPipe.setShaderUniform("matrixUni");
	shaderPipe.setShaderUniform("colourInformationUni");
	shaderPipe.setShaderUniform("sourcesUni");

	shaderPipe.linkPipeLine();
	shaderProgram = shaderPipe.getShaderProgram();

	lightSourceId = render::Sources::getInstance().addLightSource(location, 0.5f);
}

void Star::update() {
	Planet::update();

	std::lock_guard<std::mutex> guard(locationMutex);

	render::Sources::getInstance().moveLightSource(lightSourceId, location);
}

void Star::render(glm::mat4& parentMatrix) const {
	//Update the MVP-matrix and buffer this to the UBO
	matrixMutex.lock();
	glm::mat4 mVMatrix = parentMatrix * modelMatrix;
	matrixMutex.unlock();

	render::MatrixUniform matrixUni(mVMatrix);

	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(render::MatrixUniform), &matrixUni, GL_DYNAMIC_DRAW);

	//Actual rendering calls
	glBindVertexArray(vao);
	glUseProgram(shaderProgram);

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, matrixUBO);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, colourUBO);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, render::Sources::getInstance().getUBO());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vBuffers.getIBO());
	glDrawElements(GL_TRIANGLES, vBuffers.getNumIndices(), GL_UNSIGNED_INT, 0);
}
