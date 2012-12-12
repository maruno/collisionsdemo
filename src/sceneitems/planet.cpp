#include "sceneitems/planet.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "modelloader/vertexbufferfactory.hpp"

using namespace sceneitems;

Planet::Planet(glm::vec3 initialLocation, unsigned int mySize, float density)
	: bSphere(size, initialLocation), scene::GravitationalObject(initialLocation, mySize* density * 10000, "sphere", bSphere),
	  size(mySize) {
}

void Planet::move() {
	scene::SceneItem::move();

	matrixMutex.lock();
	modelMatrix = glm::scale(modelMatrix, glm::vec3(size));
	matrixMutex.unlock();
}
