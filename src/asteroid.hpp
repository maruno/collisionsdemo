#pragma once

#include <memory>

#include <dispatch/dispatch.h>

#include "renderer/scene/gravitationalobject.hpp"

#include "collisiondetection/collidable.hpp"

/**
 * Basic brown gravitational asteroid.
 */
class Asteroid : public scene::GravitationalObject, public collisiondetection::Collidable, public std::enable_shared_from_this<Asteroid> {
 private:
	dispatch_once_t removeOnceToken;
 public:
	Asteroid(glm::vec3 initialLocation, unsigned int mass);

	virtual void handleCollision(scene::SceneItem& collidee) override;

	inline std::shared_ptr<Asteroid> getShared();
};

std::shared_ptr<Asteroid> Asteroid::getShared() {
	return shared_from_this();
}