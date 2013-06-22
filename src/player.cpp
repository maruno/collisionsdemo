#include "player.hpp"

#include <mutex>
#include <typeinfo>
#include <cmath>

#include <dispatch/dispatch.h>

#include "config/globals.hpp"

#include "renderer/scene/perspectivecamera.hpp"
#include "renderer/scene/scenemanager.hpp"

#include "collisiondetection/vclip.hpp"

#include "rocket.hpp"

#warning Debug iostream include
#include <iostream>

extern dispatch_queue_t gcd_queue;
extern std::chrono::milliseconds time_since_last_update;

extern std::atomic<bool> gameOver;

extern scene::SceneManager* sceneManagerPtr;

std::shared_ptr<Player> Player::instance;

Player::Player() : render::ColouredPhongSceneItem(glm::vec3{0.0f, 0.0f, 0.0f}, "player", render::ColourInformationUniform{glm::vec3{120.0f, 0.0f, 0.0f}, 1.0f}) {
	scene::PerspectiveCamera::getInstance().changeCameraPosition(glm::vec3{0.0, 0.0f, 1.5f}, location);

	scale = glm::vec3{0.25f, 0.25f, 0.25f};
}

void Player::update() {
	float stepSize = (time_since_last_update.count() * 1000.0f) * 0.00000015f;

	signed char pitching = this->pitching;
	signed char yawing = this->yawing;

	if(pitching) {
		rotation = rotation * glm::quat{glm::vec3{pitching * stepSize, 0.0f, 0.0f}};
	}

	if(yawing) {
		rotation = rotation * glm::quat{glm::vec3{0.0f, yawing * stepSize, 0.0f}};
	}

	//Forward movement
	glm::vec3 forwardStep = glm::mat3_cast(rotation) * glm::vec3{0.0f, 0.0f, -stepSize};
	glm::vec3 newLocation = location + forwardStep;

	//Lazy camera trail
	auto when = dispatch_time(DISPATCH_TIME_NOW, 250000000);
	dispatch_after(when, gcd_queue, ^{
		//Let camera trail
		scene::PerspectiveCamera& camera = scene::PerspectiveCamera::getInstance();

		glm::vec3 newCameraPosition = newLocation - 1.5f * glm::normalize(forwardStep);
		camera.changeCameraPosition(newCameraPosition, newLocation);

		if(pitching) {
			//Update up-vector
			camera.changeUpVector(glm::mat3_cast(rotation) * glm::vec3{0.0f, 1.0f, 0.0f});
		}
	});

	//Finalise location update
	std::lock_guard<std::mutex> lock(locationMutex);
	location = newLocation;
}

void Player::yaw(signed char yaw) {
	yawing = yaw;

	//TODO Do something funny with roll for effect (GCD timer source that rolls towards a certain angle and stays till roll back by cancel)
}

void Player::pitch(signed char pitch) {
	pitching = pitch;
}

void Player::handleCollision(scene::SceneItem& collidee) {
	if(typeid(collidee) == typeid(Rocket)) {
		return;
	}

	static scene::SceneItem* lastVClipCollidee;
	static scene::SceneItem* lastCollidee;

	if (lastCollidee != &collidee) {
		lastCollidee = & collidee;

		std::cout << "OBB-collision! Collidee: " << lastCollidee << std::endl;
	}

	collisiondetection::VClip vclip(this, &collidee);
	if(vclip.run()) {
		if(lastVClipCollidee != &collidee) {
			lastVClipCollidee = &collidee;

			std::cout << "VClip-collision! Collidee: " << lastVClipCollidee << std::endl;
			gameOver = true;
		}
	}
}

void Player::fireRocket() {
	std::shared_ptr<scene::SceneItem> rocket(new Rocket(getLocation(), getRotation()));

	sceneManagerPtr->addItem(rocket);
}