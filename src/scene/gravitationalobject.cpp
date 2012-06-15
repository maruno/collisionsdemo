#include "scene/gravitationalobject.hpp"

#include "config/globals.hpp"

using namespace scene;

GravitationalObject::GravitationalObject(unsigned int myMass) : mass(myMass) {
}

void GravitationalObject::update() {
	//Newton's second law of motion
	float a = glm::length<float>(gravitationalForce) / mass;
	glm::vec3 directionalAcceleration = a * glm::normalize(gravitationalForce);
	
	//Newton's first law of motion
	currentMotion += 1.0f/Globals::updateRate * directionalAcceleration;
}