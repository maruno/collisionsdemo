#pragma once

#include "renderer/scene/gravitationalobject.hpp"

/**
 * Basic brown gravitational asteroid.
 */
class Asteroid : public scene::GravitationalObject {
 public:
	Asteroid(glm::vec3 initialLocation, unsigned int mass);
};