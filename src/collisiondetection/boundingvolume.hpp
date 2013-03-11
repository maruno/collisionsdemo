#pragma once

#include <glm/glm.hpp>

namespace scene {
	class SceneItem;
}

namespace collisiondetection {
	class BoundingVolume {
	 public:
		virtual bool intersects(const BoundingVolume& other) const = 0;

		virtual void attachToItem(scene::SceneItem* item) const = 0;

		virtual ~BoundingVolume() = default;
	};
}