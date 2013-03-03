#include "player.hpp"

#include <mutex>

#include "config/globals.hpp"

#include "renderer/scene/perspectivecamera.hpp"

std::shared_ptr<Player> Player::instance;

Player::Player() : render::ColouredPhongSceneItem(glm::vec3{0.0f, 0.0f, 0.0f}, "player", render::ColourInformationUniform{glm::vec3{120.0f, 0.0f, 0.0f}, 1.0f}) {
	scene::PerspectiveCamera::getInstance().changeCameraPosition(glm::vec3{0.0, 0.5f, 2.5f}, location);

	scale = glm::vec3{0.25f, 0.25f, 0.25f};
}

void Player::update() {
	float stepSize = (0.1f / config::globals::updateRate);

	std::lock_guard<std::mutex> lock(locationMutex);
	location.z -= stepSize;

	//Let camera trail
	scene::PerspectiveCamera& camera = scene::PerspectiveCamera::getInstance();

	glm::vec3 newCameraPosition(camera.getPosition());
	newCameraPosition.z -= stepSize;
	camera.changeCameraPosition(newCameraPosition, location);
}