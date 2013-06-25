#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "renderer/render/colouredphongsceneitem.hpp"

class Explosion : public render::ColouredPhongSceneItem, public std::enable_shared_from_this<Explosion> {
 public:
	Explosion(glm::vec3 initialLocation);

	virtual void update() override;
};