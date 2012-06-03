#include "scene/gravitationalobject.hpp"

using namespace scene;

GravitationalObject::GravitationalObject(unsigned int myMass) : mass(myMass) {
}

glm::vec3 GravitationalObject::currentMotion() const {
	//Newton's second law of motion
	float a = glm::length<float>(gravitationalForce) / mass;

	return a * glm::normalize(gravitationalForce);
}
