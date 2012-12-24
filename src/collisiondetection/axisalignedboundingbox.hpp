#pragma once

#include <tuple>

#include "collisiondetection/boundingvolume.hpp"
#include "glm/glm.hpp"

namespace collisiondetection {
	class AxisAlignedBoundingBox : public BoundingVolume {
	 public:
		const float minX, maxX;
		const float minY, maxY;
		const float minZ, maxZ;


		AxisAlignedBoundingBox(std::tuple<glm::vec3, glm::vec3> diagonal);
		virtual ~AxisAlignedBoundingBox();

		virtual bool intersects(const BoundingVolume& other) const override;
	};

	typedef AxisAlignedBoundingBox AABB;
}

