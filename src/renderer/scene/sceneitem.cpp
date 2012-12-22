#include "sceneitem.hpp"

#include <mutex>

#include <glm/gtc/matrix_transform.hpp>

#include "modelloader/vertexbufferfactory.hpp"

using namespace scene;

SceneItem::SceneItem(glm::vec3 initialLocation, std::string objectName, collisiondetection::BoundingVolume& myBounds)
	: location(initialLocation), modelMatrix{1.0f}, bounds(myBounds),
	  vBuffers(modelloader::VertexBufferFactory::getInstance()["sphere"]) {
	//Generate UBO for storing a matrix
	glGenBuffers(1, &matrixUBO);
}

void SceneItem::move() {
	std::unique_lock<std::recursive_mutex> matrixLock(matrixMutex, std::defer_lock);
	std::unique_lock<std::mutex> locationLock(locationMutex, std::defer_lock);
	
	std::lock(matrixLock, locationLock);
	
	//Move object in modelMatrix
	modelMatrix = glm::translate(glm::mat4(1.0f), location);
}
