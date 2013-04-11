#include "objectorientedboundingbox.hpp"

#include <typeinfo>
#include <cassert>

#include "renderer/scene/sceneitem.hpp"

#include <iostream>

using namespace collisiondetection;

ObjectOrientedBoundingBox::ObjectOrientedBoundingBox(std::tuple<glm::vec3, glm::vec3> myDiagonal) : objectDiagonal(myDiagonal), attachedItem{nullptr}{
	objectBoxSize.x = glm::distance(std::get<0>(objectDiagonal).x, std::get<1>(objectDiagonal).x);
	objectBoxSize.y = glm::distance(std::get<0>(objectDiagonal).y, std::get<1>(objectDiagonal).y);
	objectBoxSize.z = glm::distance(std::get<0>(objectDiagonal).z, std::get<1>(objectDiagonal).z);
}

AABB ObjectOrientedBoundingBox::toAABB() const {
	std::array<glm::vec3, 3> axis;
	glm::mat3 rotationMatrix = glm::mat3_cast(attachedItem->getRotation());

	//x
	axis[0] = glm::vec3{1.0f, 0.0f, 0.0f};
	axis[0] = axis[0] * rotationMatrix;
	axis[0] = glm::normalize(axis[0]);

	//y
	axis[1] = glm::vec3{0.0f, 1.0f, 0.0f};
	axis[1] = axis[1] * rotationMatrix;
	axis[1] = glm::normalize(axis[1]);

	//z
	axis[2] = glm::vec3{0.0f, 0.0f, 1.0f};
	axis[2] = axis[2] * rotationMatrix;
	axis[2] = glm::normalize(axis[2]);

	//Create an AABB from this OBB
	const glm::vec3 flipper{-1.0f, -1.0f, -1.0f};

	axis[0] *= halfBoxSize.x;
	glm::vec3 normalX = axis[0] + origin;
	glm::vec3 flipX = axis[0] * flipper;
	flipX += origin;

	axis[1] *= halfBoxSize.y;
	glm::vec3 normalY = axis[1] + origin;
	glm::vec3 flipY = axis[1] * flipper;
	flipY += origin;

	axis[2] *= halfBoxSize.z;
	glm::vec3 normalZ = axis[2] + origin;
	glm::vec3 flipZ = axis[2] * flipper;
	flipZ += origin;

	auto minMaxX = std::minmax({normalX.x, flipX.x, normalY.x, flipY.x, normalZ.x, flipZ.x});
	auto minMaxY = std::minmax({normalX.y, flipX.y, normalY.y, flipY.y, normalZ.y, flipZ.y});
	auto minMaxZ = std::minmax({normalX.z, flipX.z, normalY.z, flipY.z, normalZ.z, flipZ.z});

	return {std::make_tuple(glm::vec3{minMaxX.first, minMaxY.first, minMaxZ.second}, glm::vec3{minMaxX.second, minMaxY.second, minMaxZ.first})};
}

bool ObjectOrientedBoundingBox::intersects(const BoundingVolume& other) const {
	assert(attachedItem != nullptr);

	if(typeid(other) == typeid(AxisAlignedBoundingBox)) {
		return toAABB().intersects(other);
	}

	if(typeid(other) != typeid(ObjectOrientedBoundingBox)) {
		return false;
	}

	const ObjectOrientedBoundingBox& otherOBB = static_cast<const ObjectOrientedBoundingBox&>(other);

	assert(otherOBB.attachedItem != nullptr);

	return toAABB().intersects(otherOBB.toAABB());
}

void ObjectOrientedBoundingBox::attachToItem(scene::SceneItem* item) const {
	attachedItem = item;

	const glm::mat4& modelMatrix = attachedItem->getModelMatrix();

	std::tuple<glm::vec3, glm::vec3> diagonal(objectDiagonal);
	glm::vec4 diagonal1v4 = glm::vec4(std::get<0>(diagonal), 1.0f) * modelMatrix;
	glm::vec4 diagonal2v4 = glm::vec4(std::get<1>(diagonal), 1.0f) * modelMatrix;

	std::get<0>(diagonal) = glm::vec3(diagonal1v4) / diagonal1v4.w;
	std::get<1>(diagonal) = glm::vec3(diagonal2v4) / diagonal2v4.w;

	halfBoxSize = objectBoxSize;
	halfBoxSize *= attachedItem->getScale();

	origin = std::get<0>(diagonal) + 0.5f * (std::get<1>(diagonal) - std::get<0>(diagonal));
	origin += attachedItem->getLocation();
}