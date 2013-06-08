#include "gravitationalobject.hpp"

#include <string>

#include "config/globals.hpp"

using namespace scene;

extern std::chrono::milliseconds time_since_last_update;

GravitationalObject::GravitationalObject(glm::vec3 initialLocation, unsigned int myMass, std::string objectName, render::ColourInformationUniform colour)
: ColouredPhongSceneItem(initialLocation, objectName, colour), mass{myMass} {
}

void GravitationalObject::update() {
	//Newton's second law of motion
	float a = glm::length<float>(gravitationalForce) / mass;
	glm::vec3 directionalAcceleration = a * glm::normalize(gravitationalForce);

	//Newton's first law of motion
	currentMotion += (time_since_last_update.count() * 1000.0f) * directionalAcceleration;
	
	//Update location
	locationMutex.lock();
	location += currentMotion;
	locationMutex.unlock();
}
