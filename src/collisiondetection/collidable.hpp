#pragma once

#include "renderer/scene/sceneitem.hpp"

namespace collisiondetection {
	class Collidable {
	 public:
		/**
		 * Handle a collision with the given sceneitem
		 *
		 * \param collidee Sceneitem with which the collision has occured
		 */
		virtual void handleCollision(scene::SceneItem& collidee){};
	};
}