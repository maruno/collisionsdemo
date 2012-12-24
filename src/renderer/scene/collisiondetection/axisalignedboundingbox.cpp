#include "AxisAlignedBoundingBox.hpp"

#include <typeinfo>

#include "util/comparison.hpp"

/*
 float minX const { return points[0].x; };
 float minY const { return points[2].y; };
 float minZ const { return points[4].z; };
 float maxX const { return points[1].x; };
 float maxY const { return points[0].y; };
 float maxZ const { return points[0].z; };

 */

scene::collisiondetection::AxisAlignedBoundingBox::AxisAlignedBoundingBox(std::tuple<glm::vec3, glm::vec3> diagonal) {
	glm::vec3& frontLowerLeft = std::get<0>(diagonal);
	glm::vec3& backUpperRight = std::get<1>(diagonal);

	minX = frontLowerLeft.x;
	maxX = backUpperRight.x;

	minY = frontLowerLeft.y;
	maxY = backUpperRight.y;

	minZ = backUpperRight.z;
	maxZ = frontLowerLeft.z;
}

scene::collisiondetection::AxisAlignedBoundingBox::~AxisAlignedBoundingBox() {
}

bool scene::collisiondetection::AxisAlignedBoundingBox::intersects(const scene::collisiondetection::BoundingVolume& other) const {
	if(typeid(other) == typeid(AxisAlignedBoundingBox)) {
		const AxisAlignedBoundingBox& otherbox = static_cast<const AxisAlignedBoundingBox&>(other);
		
		if(util::between(minX, otherbox.minX, otherbox.maxX) || util::between(maxX, otherbox.minX, otherbox.maxX)) {
			if(util::between(minY, otherbox.minY, otherbox.maxY) || util::between(maxY, otherbox.minY, otherbox.maxY)) {
				if(util::between(minZ, otherbox.minZ, otherbox.maxZ) || util::between(maxZ, minZ, maxZ)) {
					return true;
				}
			}
		}
	}

	return false;
}