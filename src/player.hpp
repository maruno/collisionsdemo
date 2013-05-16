#pragma once

#include "renderer/render/colouredphongsceneitem.hpp"

#include "collisiondetection/collidable.hpp"

#include <memory>

class Player : public render::ColouredPhongSceneItem, public collisiondetection::Collidable {
 private:
	static std::shared_ptr<Player> instance;

	Player();
 public:
	inline static std::shared_ptr<Player> getInstance();

	virtual void update() override;

	virtual void handleCollision(scene::SceneItem& collidee) override;
};

std::shared_ptr<Player> Player::getInstance() {
	if(instance.get() == nullptr) {
		instance.reset(new Player());
	}

	return instance;
}