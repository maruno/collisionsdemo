#include "scene/gravitationalobject.hpp"

#include "config/globals.hpp"

using namespace scene;

GravitationalObject::GravitationalObject(glm::vec3 initialLocation, unsigned int myMass)
	: SceneItem(initialLocation) , mass(myMass) {
}

void GravitationalObject::update() {
	//Newton's second law of motion
	float a = glm::length<float>(gravitationalForce) / mass;
	glm::vec3 directionalAcceleration = a * glm::normalize(gravitationalForce);

	//Newton's first law of motion
	currentMotion += 1.0f/config::globals::updateRate * directionalAcceleration;
	
	//Update location
	locationMutex.lock();
	location += currentMotion;
	locationMutex.unlock();
}
