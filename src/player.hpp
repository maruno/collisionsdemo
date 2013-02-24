#pragma once

#include "renderer/render/colouredphongsceneitem.hpp"

class Player : public render::ColouredPhongSceneItem {
 public:
	Player();

	virtual void update() override;
};