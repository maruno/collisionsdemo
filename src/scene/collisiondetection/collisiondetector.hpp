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
		bool intersects(const BoundingVolume& a, const BoundingVolume& b);

		bool intersects(const scene::collisiondetection::BoundingVolume& a, const scene::collisiondetection::BoundingVolume& b);

		bool intersects(const AxisAlignedBoundingCuboid& a, const AxisAlignedBoundingCuboid& b);

		bool intersects(const BoundingSphere& a, const BoundingSphere& b);

		bool intersects(const AxisAlignedBoundingCuboid& cuboid, const BoundingSphere& sphere);
	}
}


#endif /* COLLISIONDETECTOR_HPP_ */
