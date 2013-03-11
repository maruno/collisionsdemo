#pragma once

#include "boundingvolume.hpp"

#include <tuple>
#include <array>

#include <glm/glm.hpp>

namespace collisiondetection {
	class ObjectOrientedBoundingBox : public BoundingVolume {
	 private:
		std::tuple<glm::vec3, glm::vec3> objectDiagonal;
		mutable glm::vec3 objectBoxSize;

		mutable glm::vec3 origin;
		mutable glm::vec3 halfBoxSize;

		mutable scene::SceneItem* attachedItem;
	 public:
		ObjectOrientedBoundingBox(std::tuple<glm::vec3, glm::vec3> myDiagonal);

		virtual bool intersects(const BoundingVolume& other) const override;
		virtual void attachToItem(scene::SceneItem* item) const override;
	};
}