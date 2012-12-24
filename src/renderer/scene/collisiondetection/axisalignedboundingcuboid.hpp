/*
 * AxisAlignedBoundingCuboid.hpp
 *
 *      Author: Marcel Veltman
 */

#ifndef AXISALIGNEDBOUNDINGCUBOID_HPP_
#define AXISALIGNEDBOUNDINGCUBOID_HPP_

#include <tuple>

#include "renderer/scene/collisiondetection/boundingvolume.hpp"
#include "glm/glm.hpp"

namespace scene {

	namespace collisiondetection {

		class AxisAlignedBoundingCuboid : public BoundingVolume {
		 public:
			AxisAlignedBoundingCuboid(std::tuple<glm::vec3, glm::vec3> diagonal);
			virtual ~AxisAlignedBoundingCuboid();
			float getMinX() const { return points[0].x; };
			float getMinY() const { return points[2].y; };
			float getMinZ() const { return points[4].z; };
			float getMaxX() const { return points[1].x; };
			float getMaxY() const { return points[0].y; };
			float getMaxZ() const { return points[0].z; };

			virtual bool intersects(const BoundingVolume& other) const override;
		 private:
			glm::vec3 points[8];
		};
	}
}


#endif /* AXISALIGNEDBOUNDINGCUBOID_HPP_ */
