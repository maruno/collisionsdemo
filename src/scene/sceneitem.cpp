#include "scene/sceneitem.hpp"

using namespace scene;

SceneItem::SceneItem(glm::vec3 initialLocation, collisiondetection::BoundingVolume& myBounds)
	: location(initialLocation), modelMatrix(1.0f), bounds(myBounds) {
	//Generate UBO for storing a matrix
	glGenBuffers(1, &matrixUBO);
}
