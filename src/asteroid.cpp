#include "asteroid.hpp"

Asteroid::Asteroid(glm::vec3 initialLocation, unsigned int mass) : scene::GravitationalObject(initialLocation, mass, "sphere", render::ColourInformationUniform{glm::vec3{142.0f, 100.0f, 0.0f}, 1.0f}) {
	scale = glm::vec3{mass * 0.1f, mass * 0.1f, mass * 0.1f};
}