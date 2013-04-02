#include "objectorientedboundingbox.hpp"

#include <typeinfo>
#include <cassert>

#include "axisalignedboundingbox.hpp"

#include "renderer/scene/sceneitem.hpp"

using namespace collisiondetection;

ObjectOrientedBoundingBox::ObjectOrientedBoundingBox(std::tuple<glm::vec3, glm::vec3> myDiagonal) : objectDiagonal(myDiagonal), attachedItem{nullptr}{
	objectBoxSize.x = glm::distance(std::get<0>(objectDiagonal).x, std::get<1>(objectDiagonal).x);
	objectBoxSize.y = glm::distance(std::get<0>(objectDiagonal).y, std::get<1>(objectDiagonal).y);
	objectBoxSize.z = glm::distance(std::get<0>(objectDiagonal).z, std::get<1>(objectDiagonal).z);
}

bool ObjectOrientedBoundingBox::intersects(const BoundingVolume& other) const {
	assert(attachedItem != nullptr);

	std::array<glm::vec3, 15> seperationAxis;

	glm::mat3 rotationMatrix = glm::mat3_cast(attachedItem->getRotation());

	//Ax
	seperationAxis[0] = glm::vec3{1.0f, 0.0f, 0.0f};
	seperationAxis[0] = seperationAxis[0] * rotationMatrix;
	seperationAxis[0] = glm::normalize(seperationAxis[0]);
	
	//Ay
	seperationAxis[1] = glm::vec3{0.0f, 1.0f, 0.0f};
	seperationAxis[1] = seperationAxis[1] * rotationMatrix;
	seperationAxis[1] = glm::normalize(seperationAxis[1]);


	//Az
	seperationAxis[2] = glm::vec3{0.0f, 0.0f, 1.0f};
	seperationAxis[2] = seperationAxis[2] * rotationMatrix;
	seperationAxis[2] = glm::normalize(seperationAxis[2]);

	if(typeid(other) == typeid(AxisAlignedBoundingBox)) {
		//Create an AABB from this OBB and use this for checking collisions
		seperationAxis[0] *= halfBoxSize.x;
		seperationAxis[0] += origin;

		seperationAxis[1] *= halfBoxSize.y;
		seperationAxis[1] += origin;

		seperationAxis[2] *= halfBoxSize.z;
		seperationAxis[2] += origin;

		auto minMaxX = std::minmax({seperationAxis[0].x, seperationAxis[1].x, seperationAxis[2].x});
		auto minMaxY = std::minmax({seperationAxis[0].y, seperationAxis[1].y, seperationAxis[2].y});
		auto minMaxZ = std::minmax({seperationAxis[0].z, seperationAxis[1].z, seperationAxis[2].z});

		AABB axisAligned{std::make_tuple(glm::vec3{minMaxX.first, minMaxY.first, minMaxZ.second}, glm::vec3{minMaxX.second, minMaxY.second, minMaxZ.first})};

		return axisAligned.intersects(other);
	}

	if(typeid(other) != typeid(ObjectOrientedBoundingBox)) {
		return false;
	}

	const ObjectOrientedBoundingBox& otherOBB = static_cast<const ObjectOrientedBoundingBox&>(other);

	assert(otherOBB.attachedItem != nullptr);

	glm::mat3 otherRotationMatrix = glm::mat3_cast(otherOBB.attachedItem->getRotation());

	//Bx-axis
	seperationAxis[3] = glm::vec3{1.0f, 0.0f, 0.0f};
	seperationAxis[3] = seperationAxis[3] * otherRotationMatrix;
	seperationAxis[3] = glm::normalize(seperationAxis[3]);
	
	//By-axis
	seperationAxis[4] = glm::vec3{0.0f, 1.0f, 0.0f};
	seperationAxis[4] = seperationAxis[4] * otherRotationMatrix;
	seperationAxis[4] = glm::normalize(seperationAxis[4]);
	
	//Bz-axis
	seperationAxis[5] = glm::vec3{0.0f, 0.0f, 1.0f};
	seperationAxis[5] = seperationAxis[5] * otherRotationMatrix;
	seperationAxis[5] = glm::normalize(seperationAxis[5]);
	
	//Ax x Bx
	seperationAxis[6] = glm::cross(seperationAxis[0], seperationAxis[3]);

	//Ax x By
	seperationAxis[7] = glm::cross(seperationAxis[0], seperationAxis[4]);

	//Ax x Bz
	seperationAxis[8] = glm::cross(seperationAxis[0], seperationAxis[5]);

	//Ay x Bx
	seperationAxis[9] = glm::cross(seperationAxis[1], seperationAxis[3]);

	//Ay x By
	seperationAxis[10] = glm::cross(seperationAxis[1], seperationAxis[4]);

	//Ay x Bz
	seperationAxis[11] = glm::cross(seperationAxis[1], seperationAxis[5]);

	//Az x Bx
	seperationAxis[12] = glm::cross(seperationAxis[2], seperationAxis[3]);

	//Az x By
	seperationAxis[13] = glm::cross(seperationAxis[2], seperationAxis[4]);

	//Az x Bz
	seperationAxis[14] = glm::cross(seperationAxis[2], seperationAxis[5]);
	
	for(glm::vec3& l : seperationAxis) {
		bool overlap = glm::abs(glm::dot((otherOBB.origin - origin), l)) > glm::abs(halfBoxSize.x * glm::dot(seperationAxis[0], l)) + glm::abs(halfBoxSize.y * glm::dot(seperationAxis[1], l)) + glm::abs(halfBoxSize.z * glm::dot(seperationAxis[2], l)) + glm::abs(otherOBB.halfBoxSize.x * glm::dot(seperationAxis[3], l)) + glm::abs(otherOBB.halfBoxSize.y * glm::dot(seperationAxis[4], l)) + glm::abs(otherOBB.halfBoxSize.z * glm::dot(seperationAxis[5], l));

		if(!overlap) {
			return false;
		}
	}

	return true;
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