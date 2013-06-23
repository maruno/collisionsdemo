#include "asteroid.hpp"

#include <typeinfo>

#include <dispatch/dispatch.h>

#include "renderer/scene/scenemanager.hpp"

#include "rocket.hpp"

extern scene::SceneManager* sceneManagerPtr;

Asteroid::Asteroid(glm::vec3 initialLocation, unsigned int mass) : scene::GravitationalObject(initialLocation, mass, "sphere", render::ColourInformationUniform{glm::vec3{142.0f, 100.0f, 0.0f}, 1.0f}), removeOnceToken{0} {
	scale = glm::vec3{mass * 0.5f, mass * 0.5f, mass * 0.5f};
}

void Asteroid::handleCollision(scene::SceneItem& collidee) {
	if (typeid(collidee) == typeid(Rocket)) {
		static_cast<Rocket&>(collidee).explode();

		dispatch_once(&removeOnceToken, ^{
			dispatch_async(dispatch_get_current_queue(), ^{
				sceneManagerPtr->removeItem(getShared());
			});
		});

		return;
	}

	if (typeid(collidee) != typeid(*this)) {
		return;
	}

	Asteroid& other = static_cast<Asteroid&>(collidee);

	//TODO Sphere collision test
	float distance = glm::distance(location, other.location);

	if ((distance - other.scale.x) > scale.x) {
		return;
	}

	//Calculate orthonormal basis
	const glm::vec3 N = glm::normalize(other.location - location);
	glm::vec3 O = glm::cross(N, glm::vec3{1.0f, 0.0f, 0.0f});

	float magnitudeO = glm::length(O);
	if(magnitudeO > -0.01f && magnitudeO < 0.01f) {
		O = glm::cross(N, glm::vec3{0.0f, 1.0f, 0.0f});
	}
	O = glm::normalize(O);

	glm::vec3 T = glm::normalize(glm::cross(N, O));

	//Elastic collision
	glm::vec3 v1N{glm::dot(N, currentMotion), glm::dot(T, currentMotion), glm::dot(O, currentMotion)};
	glm::vec3 v2N{glm::dot(N, other.currentMotion), glm::dot(T, other.currentMotion), glm::dot(O, other.currentMotion)};

	glm::vec3 newMotion, otherNewMotion;

	newMotion.x = (int(mass) - int(other.mass))/(int(mass) + int(other.mass))*v1N.x + (2*int(other.mass))/(int(mass) + int(other.mass))*v2N.x;
	newMotion.y = (int(mass) - int(other.mass))/(int(mass) + int(other.mass))*v1N.y + (2*int(other.mass))/(int(mass) + int(other.mass))*v2N.y;
	newMotion.z = (int(mass) - int(other.mass))/(int(mass) + int(other.mass))*v1N.z + (2*int(other.mass))/(int(mass) + int(other.mass))*v2N.z;

	otherNewMotion.x = (2*int(mass))/(int(mass) + int(other.mass))*v1N.x + (int(other.mass) - int(mass))/(int(mass) + int(other.mass))*v2N.x;
	otherNewMotion.y = (2*int(mass))/(int(mass) + int(other.mass))*v1N.y + (int(other.mass) - int(mass))/(int(mass) + int(other.mass))*v2N.y;
	otherNewMotion.z = (2*int(mass))/(int(mass) + int(other.mass))*v1N.x + (int(other.mass) - int(mass))/(int(mass) + int(other.mass))*v2N.z;

	currentMotion = newMotion.x * N + newMotion.y * T + newMotion.z * O;
	other.currentMotion = otherNewMotion.x * N + otherNewMotion.y * T + otherNewMotion.z * O;
}