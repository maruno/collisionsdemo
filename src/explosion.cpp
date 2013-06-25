#include "explosion.hpp"

#include <dispatch/dispatch.h>

#include "renderer/scene/scenemanager.hpp"

extern std::chrono::milliseconds time_since_last_update;

extern scene::SceneManager* sceneManagerPtr;

Explosion::Explosion(glm::vec3 initialLocation) : render::ColouredPhongSceneItem(initialLocation, "sphere", render::ColourInformationUniform{glm::vec3{120.0f, 0.0, 0.0f}, 1.0f}) {
	scale = glm::vec3{0.1f, 0.1f, 0.1f};
}

void Explosion::update() {
	float stepSize = (time_since_last_update.count() * 1000.0f) * 0.0000035f;

	scale += stepSize;

	if(scale.x >= 0.8f) {
		dispatch_async(dispatch_get_current_queue(), ^{
			sceneManagerPtr->removeItem(shared_from_this());
		});
	}
}
