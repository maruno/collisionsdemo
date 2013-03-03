#pragma once

#include "renderer/render/colouredphongsceneitem.hpp"

#include <memory>

class Player : public render::ColouredPhongSceneItem {
 private:
	static std::shared_ptr<Player> instance;

	Player();
 public:
	inline static std::shared_ptr<Player> getInstance();

	virtual void update() override;
};

std::shared_ptr<Player> Player::getInstance() {
	if(instance.get() == nullptr) {
		instance.reset(new Player());
	}

	return instance;
}