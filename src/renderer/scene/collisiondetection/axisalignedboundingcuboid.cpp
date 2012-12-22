/*
 * AxisAlignedBoundingCuboid.cpp
 *
 *      Author: Marcel Veltman
 */

#include "axisalignedboundingcuboid.hpp"

scene::collisiondetection::AxisAlignedBoundingCuboid::AxisAlignedBoundingCuboid(std::tuple<glm::vec3, glm::vec3> diagonal) {
	glm::vec3 ex1 = std::get<0>(diagonal);
	glm::vec3 ex2 = std::get<1>(diagonal);

	points[0] = glm::vec3(ex1.x, ex2.y, ex1.z);// front upper left
	points[1] = glm::vec3(ex2.x, ex2.y, ex1.z);// front upper right
	points[2] = glm::vec3(ex1.x, ex1.y, ex1.z);// front lower left
	points[3] = glm::vec3(ex2.x, ex1.y, ex1.z);// front lower right
	points[4] = glm::vec3(ex1.x, ex2.y, ex2.z);// back upper left
	points[5] = glm::vec3(ex2.x, ex2.y, ex2.z);// back upper right
	points[6] = glm::vec3(ex1.x, ex1.y, ex2.z);// back lower left
	points[7] = glm::vec3(ex2.x, ex1.y, ex2.z);// back lower right
}

scene::collisiondetection::AxisAlignedBoundingCuboid::~AxisAlignedBoundingCuboid() {
}
