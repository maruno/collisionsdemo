#include "scene/perspectivecamera.hpp"

#include "glload/gl_3_2.h"

#include <glm/gtc/matrix_transform.hpp>

#include "scene/scenegroup.hpp"

#include "config/globals.hpp"
#include "render/lightmanager.hpp"

using namespace scene;

const glm::vec3 PerspectiveCamera::up(0.0f, 1.0f, 0.0f);
std::unique_ptr<PerspectiveCamera> PerspectiveCamera::instance;

PerspectiveCamera::PerspectiveCamera() : direction{0.0f, 0.0f, -1.0f}, needsUpload{true} {
	glGenBuffers(1, &uBO);
}

void PerspectiveCamera::rescale(int width, int height) {
	glViewport(0, 0, width, height);

	//HOR+
	//The approximate field of view of a human eye is 95° out, 75° down, 60° in, 60° up
	projection = glm::perspective<float>(60.0f, static_cast<float>(width)/height, 0.3f, 1000.0f);
}

void PerspectiveCamera::changeCameraPosition(glm::vec3 position, glm::vec3 direction) {
	glm::vec3 lookAtCenter =  position + direction;

	view = glm::lookAt<float>(position, lookAtCenter, glm::vec3(0.0f, 1.0f, 0.0f));
}

void PerspectiveCamera::render(SceneGroup* world) {
	upload();
	
	world->visitScene([this](std::unique_ptr<SceneItem>& child) {
		child->render(view);
	});
}

void PerspectiveCamera::update() {
	constexpr float directionUpdate = 1.0f/config::globals::updateRate * config::globals::cameraRotationalSpeed;
	constexpr float positionUpdate = 1.0f/config::globals::updateRate * config::globals::cameraMovementSpeed;

	changeCameraPosition(position, direction);
}

void PerspectiveCamera::upload() {
	if(needsUpload) {
		glBindBuffer(GL_UNIFORM_BUFFER, uBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), &position, GL_DYNAMIC_DRAW);
		
		needsUpload = false;
	}
}

void PerspectiveCamera::updatePosition(float dX, float dY, float dZ) {
	position.x += dX;
	position.y += dY;
	position.z += dZ;
}

void PerspectiveCamera::updateDirection(float angle, float x, float y, float z) {
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(x, y, z));
	glm::vec4 dir(direction, 1);
	dir = rotation * dir;
	direction.x = dir.x;
	direction.y = dir.y;
	direction.z = dir.z;
}
