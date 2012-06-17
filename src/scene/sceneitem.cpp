#include "scene/sceneitem.hpp"

using namespace scene;

SceneItem::SceneItem(glm::vec3 initialLocation) : location(initialLocation), modelMatrix(1.0f) {
	//Generate UBO for storing a matrix
	glGenBuffers(1, &matrixUBO);
}