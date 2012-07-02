#include "scene/collisiondetection/collisiondetector.hpp"

using namespace scene::collisiondetection;

bool scene::collisiondetection::intersects(BoundingVolume& a, BoundingVolume& b) {
	bool collision = false;
	const std::type_info& typeA = typeid(a);
	const std::type_info& typeB = typeid(b);
	if(typeA == typeid(BoundingSphere) && typeB == typeid(BoundingSphere)) {
		BoundingSphere& bsa = static_cast<BoundingSphere&>(a);
		BoundingSphere& bsb = static_cast<BoundingSphere&>(b);
		collision = intersects(bsa, bsb);
	} else if(typeA == typeid(AxisAlignedBoundingCuboid) && typeB == typeid(AxisAlignedBoundingCuboid)) {
		AxisAlignedBoundingCuboid& aabca = static_cast<AxisAlignedBoundingCuboid&>(a);
		AxisAlignedBoundingCuboid& aabcb = static_cast<AxisAlignedBoundingCuboid&>(b);
		collision = intersects(aabca, aabcb);
	} else if(typeA == typeid(AxisAlignedBoundingCuboid) && typeB == typeid(BoundingSphere)) {
		AxisAlignedBoundingCuboid& cuboid = static_cast<AxisAlignedBoundingCuboid&>(a);
		BoundingSphere& sphere = static_cast<BoundingSphere&>(b);
		collision = intersects(cuboid, sphere);
	} else if(typeA == typeid(BoundingSphere) && typeB == typeid(AxisAlignedBoundingCuboid)) {
		BoundingSphere& sphere = static_cast<BoundingSphere&>(a);
		AxisAlignedBoundingCuboid& cuboid = static_cast<AxisAlignedBoundingCuboid&>(b);
		collision = intersects(cuboid, sphere);
	}
	return collision;
}

bool scene::collisiondetection::intersects(BoundingSphere& a, BoundingSphere& b) {
	bool collision = false;

	float xSquare = (a.getLocation().x-b.getLocation().x)*(a.getLocation().x-b.getLocation().x);
	float ySquare = (a.getLocation().y-b.getLocation().y)*(a.getLocation().y-b.getLocation().y);
	float zSquare = (a.getLocation().z-b.getLocation().z)*(a.getLocation().z-b.getLocation().z);

	float centreDistance = std::sqrt(xSquare+ySquare+zSquare);
	collision = centreDistance <= (a.getRadius() + b.getRadius());

	return collision;
}

bool scene::collisiondetection::intersects(AxisAlignedBoundingCuboid& a, AxisAlignedBoundingCuboid& b) {
	bool collision = true;

	if(a.getMinX() > b.getMaxX() || a.getMaxX() < b.getMinX()) {
		collision = false;
	} else if(a.getMinY() > b.getMaxY() || a.getMaxY() < b.getMinY()) {
		collision = false;
	} else if(a.getMinZ() > b.getMaxZ() || a.getMaxZ() < b.getMinZ()) {
		collision = false;
	}

	return collision;
}

bool scene::collisiondetection::intersects(AxisAlignedBoundingCuboid& cuboid, BoundingSphere& sphere) {
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
