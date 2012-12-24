#include "AxisAlignedBoundingBox.hpp"

#include <typeinfo>

#include "util/comparison.hpp"

using namespace collisiondetection;

AxisAlignedBoundingBox::AxisAlignedBoundingBox(std::tuple<glm::vec3, glm::vec3> diagonal)
: minX(std::get<0>(diagonal).x), maxX(std::get<1>(diagonal).x), minY(std::get<0>(diagonal).y), maxY(std::get<1>(diagonal).y), minZ(std::get<1>(diagonal).z), maxZ(std::get<0>(diagonal).z) {
}

AxisAlignedBoundingBox::~AxisAlignedBoundingBox() {
}

bool AxisAlignedBoundingBox::intersects(const BoundingVolume& other) const {
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