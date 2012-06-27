/*
 * AxisAlignedBoundingCuboid.hpp
 *
 *      Author: Marcel Veltman
 */

#ifndef AXISALIGNEDBOUNDINGCUBOID_HPP_
#define AXISALIGNEDBOUNDINGCUBOID_HPP_

#include <tuple>

#include "scene/collisiondetection/boundingvolume.hpp"
#include "glm/glm.hpp"

namespace scene {

	namespace collisiondetection {

		class AxisAlignedBoundingCuboid : public BoundingVolume {
			public:
				AxisAlignedBoundingCuboid(std::tuple<glm::vec3, glm::vec3> diagonal);
				virtual ~AxisAlignedBoundingCuboid();
				glm::vec3* getPoints(){ return points; };
				float getMinX(){ return points[0].x; };
				float getMinY(){ return points[2].y; };
				float getMinZ(){ return points[4].z; };
				float getMaxX(){ return points[1].x; };
				float getMaxY(){ return points[0].y; };
				float getMaxZ(){ return points[0].z; };
			private:
				glm::vec3 points[8];
		};
	}
}


#endif /* AXISALIGNEDBOUNDINGCUBOID_HPP_ */
