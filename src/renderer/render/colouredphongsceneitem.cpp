#include "colouredphongsceneitem.hpp"

#include <mutex>

#include "lightmanager.hpp"
#include "shaderpipeline.hpp"

#include "colourinformationuniform.hpp"
#include "matrixuniform.hpp"

#include "scene/perspectivecamera.hpp"

using namespace render;

ColouredPhongSceneItem::ColouredPhongSceneItem(glm::vec3 initialLocation, std::string objectName,
			       scene::collisiondetection::BoundingVolume& myBounds,
			       ColourInformationUniform colour)
	: scene::SceneItem(initialLocation, objectName, myBounds) {
	
	//Buffer the colour
	glGenBuffers(1, &colourInfoUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, colourInfoUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(render::ColourInformationUniform), &colour, GL_STATIC_DRAW);
	
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

void ColouredPhongSceneItem::render(glm::mat4& parentMatrix) const {
	//Update the MVP-matrix and buffer this to the UBO
	std::unique_lock<std::recursive_mutex> matrixLock(matrixMutex);
	glm::mat4 mVMatrix = parentMatrix * modelMatrix;
	matrixLock.unlock();

	render::MatrixUniform matrixUni(mVMatrix);

	glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(render::MatrixUniform), &matrixUni, GL_DYNAMIC_DRAW);

	//Actual rendering calls
	vBuffers.bindBuffers();
	glUseProgram(shaderProgram);

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, matrixUBO);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, colourInfoUBO);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, render::LightManager::getInstance().getUBO());
	glBindBufferBase(GL_UNIFORM_BUFFER, 4, scene::PerspectiveCamera::getInstance().getUBO());

	glDrawElements(GL_TRIANGLES, vBuffers.getNumIndices(), GL_UNSIGNED_INT, 0);
}
