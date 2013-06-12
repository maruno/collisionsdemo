#pragma once

#include "renderer/scene/gravitationalobject.hpp"

#include "collisiondetection/collidable.hpp"

/**
 * Basic brown gravitational asteroid.
 */
class Asteroid : public scene::GravitationalObject, public collisiondetection::Collidable {
 public:
	Asteroid(glm::vec3 initialLocation, unsigned int mass);
	virtual void handleCollision(scene::SceneItem& collidee);
};