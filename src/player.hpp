#pragma once

#include "renderer/render/colouredphongsceneitem.hpp"

#include "collisiondetection/collidable.hpp"

#include <memory>

class Player : public render::ColouredPhongSceneItem, public collisiondetection::Collidable {
 private:
	static std::shared_ptr<Player> instance;
	std::atomic<signed char> yawing, pitching;

	Player();
 public:
	inline static std::shared_ptr<Player> getInstance();

	virtual void update() override;

	void yaw(signed char yaw);
	void pitch(signed char pitch);

	virtual void handleCollision(scene::SceneItem& collidee) override;

	void fireRocket();
};

std::shared_ptr<Player> Player::getInstance() {
	if(instance.get() == nullptr) {
		instance.reset(new Player());
	}

	return instance;
}