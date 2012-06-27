/*
 * CollisionDetector.hpp
 *
 *      @author: Marcel Veltman
 */

#ifndef COLLISIONDETECTOR_HPP_
#define COLLISIONDETECTOR_HPP_

#include <typeinfo>
#include <glm/glm.hpp>

#include "scene/collisiondetection/boundingsphere.hpp"
#include "scene/collisiondetection/boundingvolume.hpp"
#include "scene/collisiondetection/axisalignedboundingcuboid.hpp"

namespace scene {

	namespace collisiondetection {
		/**
		 * Returns true when the bounding volumes collide.
		 */
		bool intersects(BoundingVolume& a, BoundingVolume& b);

		bool intersects(BoundingSphere& a, BoundingSphere& b);

		bool intersects(AxisAlignedBoundingCuboid& a, AxisAlignedBoundingCuboid& b);

		bool intersects(AxisAlignedBoundingCuboid& cuboid, BoundingSphere& sphere);
	}
}


#endif /* COLLISIONDETECTOR_HPP_ */
