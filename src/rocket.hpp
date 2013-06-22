#pragma once

#include <glm/glm.hpp>

#include "renderer/render/colouredphongsceneitem.hpp"

#include "collisiondetection/collidable.hpp"

class Rocket : public render::ColouredPhongSceneItem {
 public:
	Rocket(glm::vec3 initialLocation, glm::quat initialRotation);

	virtual void update() override;

	void explode();
};