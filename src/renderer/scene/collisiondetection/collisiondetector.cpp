#include "collisiondetector.hpp"

using namespace scene::collisiondetection;

bool scene::collisiondetection::intersects(const BoundingVolume& a, const BoundingVolume& b) {
	bool collision = false;
	const std::type_info& typeA = typeid(a);
	const std::type_info& typeB = typeid(b);
	if(typeA == typeid(const BoundingSphere) && typeB == typeid(const BoundingSphere)) {
		const BoundingSphere& bsa = static_cast<const BoundingSphere&>(a);
		const BoundingSphere& bsb = static_cast<const BoundingSphere&>(b);
		collision = intersects(bsa, bsb);
	} else if(typeA == typeid(const AxisAlignedBoundingCuboid) && typeB == typeid(const AxisAlignedBoundingCuboid)) {
		const AxisAlignedBoundingCuboid& aabca = static_cast<const AxisAlignedBoundingCuboid&>(a);
		const AxisAlignedBoundingCuboid& aabcb = static_cast<const AxisAlignedBoundingCuboid&>(b);
		collision = intersects(aabca, aabcb);
	} else if(typeA == typeid(const AxisAlignedBoundingCuboid) && typeB == typeid(const BoundingSphere)) {
		const AxisAlignedBoundingCuboid& cuboid = static_cast<const AxisAlignedBoundingCuboid&>(a);
		const BoundingSphere& sphere = static_cast<const BoundingSphere&>(b);
		collision = intersects(cuboid, sphere);
	} else if(typeA == typeid(const BoundingSphere) && typeB == typeid(const AxisAlignedBoundingCuboid)) {
		const BoundingSphere& sphere = static_cast<const BoundingSphere&>(a);
		const AxisAlignedBoundingCuboid& cuboid = static_cast<const AxisAlignedBoundingCuboid&>(b);
		collision = intersects(cuboid, sphere);
	}
	return collision;
}

bool scene::collisiondetection::intersects(const BoundingSphere& a, const BoundingSphere& b) {
	bool collision = false;

	float xSquare = (a.getLocation().x-b.getLocation().x)*(a.getLocation().x-b.getLocation().x);
	float ySquare = (a.getLocation().y-b.getLocation().y)*(a.getLocation().y-b.getLocation().y);
	float zSquare = (a.getLocation().z-b.getLocation().z)*(a.getLocation().z-b.getLocation().z);

	float centreDistance = std::sqrt(xSquare+ySquare+zSquare);
	collision = centreDistance <= (a.getRadius() + b.getRadius());

	return collision;
}

bool scene::collisiondetection::intersects(const AxisAlignedBoundingCuboid& a, const AxisAlignedBoundingCuboid& b) {
	return a.intersects(b);
}

bool scene::collisiondetection::intersects(const AxisAlignedBoundingCuboid& cuboid, const BoundingSphere& sphere) {
	bool collision = false;
	glm::vec3 closestPoint;

	if(sphere.getLocation().x < cuboid.getMinX()) {
		closestPoint.x = cuboid.getMinX();
	} else if(sphere.getLocation().x > cuboid.getMaxX()) {
		closestPoint.x = cuboid.getMaxX();
	} else {
		closestPoint.x = sphere.getLocation().x;
	}

	if(sphere.getLocation().y < cuboid.getMinY()) {
		closestPoint.y = cuboid.getMinY();
	} else if(sphere.getLocation().y > cuboid.getMaxY()) {
		closestPoint.y = cuboid.getMaxY();
	} else {
		closestPoint.y = sphere.getLocation().y;
	}

	if(sphere.getLocation().z < cuboid.getMinZ()) {
		closestPoint.z = cuboid.getMinZ();
	} else if(sphere.getLocation().z > cuboid.getMaxZ()) {
		closestPoint.z = cuboid.getMaxZ();
	} else {
		closestPoint.z = sphere.getLocation().z;
	}

	if(closestPoint == sphere.getLocation()) {
		collision = true;
	} else {
		float xSquare = closestPoint.x*closestPoint.x;
		float ySquare = closestPoint.y*closestPoint.y;
		float zSquare = closestPoint.z*closestPoint.z;
		float distanceToCentre = std::sqrt(xSquare+ySquare+zSquare);
		collision = distanceToCentre < sphere.getRadius();
	}

	return collision;
}
