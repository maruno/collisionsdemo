#include "rocket.hpp"

#include <chrono>

#include <dispatch/dispatch.h>

#include "renderer/scene/scenemanager.hpp"

extern dispatch_queue_t gcd_queue;
extern std::chrono::milliseconds time_since_last_update;

extern scene::SceneManager* sceneManagerPtr;

Rocket::Rocket(glm::vec3 initialLocation, glm::quat initialRotation) : render::ColouredPhongSceneItem(initialLocation, "rocket", render::ColourInformationUniform{glm::vec3{148.0f, 23.0f, 81.0f}, 1.0f}) {
	rotation = initialRotation;
	scale = glm::vec3{0.15f, 0.15f, 0.15f};

	auto when = dispatch_time(DISPATCH_TIME_NOW, 4000000000);
	dispatch_after(when, gcd_queue, ^{
		explode();
	});
}

void Rocket::update() {
	float stepSize = (time_since_last_update.count() * 1000.0f) * 0.0000035f;

	//Forward movement
	glm::vec3 forwardStep = glm::mat3_cast(rotation) * glm::vec3{0.0f, 0.0f, -stepSize};

	//Finalise location update
	std::lock_guard<std::mutex> lock(locationMutex);
	location = location + forwardStep;
}

void Rocket::explode() {
	sceneManagerPtr->removeItem(getShared());
}