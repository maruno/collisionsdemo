#pragma once

#include <memory>
#include <chrono>

#include <dispatch/dispatch.h>
#include <glm/glm.hpp>

#include "renderer/render/colouredphongsceneitem.hpp"

#include "collisiondetection/collidable.hpp"

class Rocket : public render::ColouredPhongSceneItem, public std::enable_shared_from_this<Rocket> {
 private:
	dispatch_once_t explodeOnceToken;
	std::chrono::milliseconds lifeTime;
 public:
	Rocket(glm::vec3 initialLocation, glm::quat initialRotation);

	virtual void update() override;

	void explode();

	inline std::shared_ptr<Rocket> getShared();
};

std::shared_ptr<Rocket> Rocket::getShared() {
	return shared_from_this();
}