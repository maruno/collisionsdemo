/*
 * BoundingSphere.cpp
 *
 *      Author: Marcel Veltman
 */

#include "scene/collisiondetection/boundingsphere.hpp"

scene::collisiondetection::BoundingSphere::BoundingSphere(float radius, glm::vec3 centreLocation) {
	this->radius = radius;
	location = centreLocation;
}

scene::collisiondetection::BoundingSphere::~BoundingSphere() {

}
