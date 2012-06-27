#include "scene/sceneitem.hpp"

#include <mutex>

#include <glm/gtc/matrix_transform.hpp>

using namespace scene;

SceneItem::SceneItem(glm::vec3 initialLocation, collisiondetection::BoundingVolume& myBounds)
	: location(initialLocation), modelMatrix(1.0f), bounds(myBounds) {
	//Generate UBO for storing a matrix
	glGenBuffers(1, &matrixUBO);
}

void SceneItem::move() {
	//Move object in modelMatrix
	std::lock(matrixMutex, locationMutex);
	
	modelMatrix = glm::translate(glm::mat4(1.0f), location);
	
	matrixMutex.unlock();
	locationMutex.unlock();
}