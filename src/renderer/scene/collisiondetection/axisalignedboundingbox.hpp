#pragma once

#include <tuple>

#include "renderer/scene/collisiondetection/boundingvolume.hpp"
#include "glm/glm.hpp"

namespace scene {
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
}
