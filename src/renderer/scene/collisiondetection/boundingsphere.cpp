/*
 * BoundingSphere.cpp
 *
 *      Author: Marcel Veltman
 */

#include "boundingsphere.hpp"

scene::collisiondetection::BoundingSphere::BoundingSphere(float radius, glm::vec3 centreLocation) {
	this->radius = radius;
	location = centreLocation;
}

scene::collisiondetection::BoundingSphere::~BoundingSphere() {

}

bool scene::collisiondetection::BoundingSphere::intersects(const scene::collisiondetection::BoundingVolume& other) const {
	return false;
}