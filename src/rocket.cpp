#include "rocket.hpp"

#include <chrono>
#include <iostream>

#include <dispatch/dispatch.h>

#include "renderer/scene/scenemanager.hpp"

#include "explosion.hpp"

extern dispatch_queue_t gcd_queue;
extern std::chrono::milliseconds time_since_last_update;

extern scene::SceneManager* sceneManagerPtr;

Rocket::Rocket(glm::vec3 initialLocation, glm::quat initialRotation) : render::ColouredPhongSceneItem(initialLocation, "rocket", render::ColourInformationUniform{glm::vec3{148.0f, 23.0f, 81.0f}, 1.0f}), lifeTime{0}, explodeOnceToken{0}, exploded{false} {
	rotation = initialRotation;
	scale = glm::vec3{0.15f, 0.15f, 0.15f};
}

void Rocket::update() {
	lifeTime += time_since_last_update;

	if (lifeTime.count() >= 4000) {
		explode(getLocation());
		return;
	}

	float stepSize = (time_since_last_update.count() * 1000.0f) * 0.0000035f;

	//Forward movement
	glm::vec3 forwardStep = glm::mat3_cast(rotation) * glm::vec3{0.0f, 0.0f, -stepSize};

	//Finalise location update
	std::lock_guard<std::mutex> lock(locationMutex);
	location = location + forwardStep;
}

void Rocket::explode(glm::vec3 location) {
	exploded = true;
	explosionLocation = location;
}

void Rocket::auxilaryOnRenderThread() {
	if (exploded) {
		dispatch_once(&explodeOnceToken, ^{
			dispatch_async(gcd_queue, ^{
				sceneManagerPtr->removeItem(getShared());
			});

			std::shared_ptr<Explosion> explosion(new Explosion(explosionLocation));
			sceneManagerPtr->addItem(explosion);
		});
	}
}