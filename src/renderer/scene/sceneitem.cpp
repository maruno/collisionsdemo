#include "sceneitem.hpp"

#include <mutex>

#include <glm/gtc/matrix_transform.hpp>

#include "modelloader/vertexbufferfactory.hpp"

using namespace scene;

SceneItem::SceneItem(glm::vec3 initialLocation, std::string objectName)
: location(initialLocation), modelMatrix{1.0f}, scale{1.0f, 1.0f, 1.0f}, vBuffers(modelloader::VertexBufferFactory::getInstance()[objectName]) {
	//Generate UBO for storing a matrix
	glGenBuffers(1, &matrixUBO);
}

void SceneItem::buildModelMatrix() {
	std::unique_lock<std::recursive_mutex> matrixLock(matrixMutex, std::defer_lock);
	std::unique_lock<std::mutex> locationLock(locationMutex, std::defer_lock);
	
	std::lock(matrixLock, locationLock);
	
	//Move object in modelMatrix
	modelMatrix = glm::translate(glm::mat4{1.0f}, location);

	//Scale
	modelMatrix = glm::scale(modelMatrix, scale);

	//Apply rotation from quaternion
	glm::mat4_cast(rotation) *= modelMatrix;
}
