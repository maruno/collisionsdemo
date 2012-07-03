#include "scene/perspectivecamera.hpp"

#include "glload/gl_3_2.h"

#include <glm/gtc/matrix_transform.hpp>

#include "scene/scenegroup.hpp"

#include "config/globals.hpp"

using namespace scene;

const glm::vec3 PerspectiveCamera::up(0.0f, 1.0f, 0.0f);
PerspectiveCamera* PerspectiveCamera::instance;
PerspectiveCamera::KeyPress PerspectiveCamera::keyPressed = NO_KEY_PRESSED;

PerspectiveCamera::PerspectiveCamera() : direction(0.0f, 0.0f, -1.0f) {
}

void PerspectiveCamera::rescale(int width, int height) {
	glViewport(0, 0, width, height);

	//HOR+
	//The approximate field of view of a human eye is 95° out, 75° down, 60° in, 60° up
	projection = glm::perspective<float>(60.0f, static_cast<float>(width)/height, 0.3f, 200.0f);

	viewProjection = projection * view;
}

void PerspectiveCamera::changeCameraPosition(glm::vec3 position, glm::vec3 direction) {
	glm::vec3 lookAtCenter =  position + direction;

	view = glm::lookAt<float>(position, lookAtCenter, glm::vec3(0.0f, 1.0f, 0.0f));

	viewProjection = projection * view;
}

void PerspectiveCamera::render(SceneGroup* world) {
	world->visitScene([this](std::unique_ptr<SceneItem>& child) {
		child->render(viewProjection);
	});
}

void PerspectiveCamera::update() {
	constexpr float directionUpdate = 1.0f/config::globals::updateRate * config::globals::cameraRotationalSpeed;
	constexpr float positionUpdate = 1.0f/config::globals::updateRate * config::globals::cameraMovementSpeed;

	switch(keyPressed) {
	case UP_KEY_PRESSED:
		updateDirection(directionUpdate, 1, 0, 0);
		break;
	case DOWN_KEY_PRESSED:
		updateDirection(-directionUpdate, 1, 0, 0);
		break;
	case LEFT_KEY_PRESSED:
		updateDirection(directionUpdate, 0, 1, 0);
		break;
	case RIGHT_KEY_PRESSED:
		updateDirection(-directionUpdate, 0, 1, 0);
		break;
	case W_KEY_PRESSED:
		updatePosition(0, 0, -positionUpdate);
		break;
	case A_KEY_PRESSED:
		updatePosition(-positionUpdate, 0, 0);
		break;
	case S_KEY_PRESSED:
		updatePosition(0, 0, positionUpdate);
		break;
	case D_KEY_PRESSED:
		updatePosition(positionUpdate, 0, 0);
		break;
	case PAGEUP_KEY_PRESSED:
		updatePosition(0, positionUpdate, 0);
		break;
	case PAGEDOWN_KEY_PRESSED:
		updatePosition(0, -positionUpdate, 0);
		break;
	default:
		;
	}

	changeCameraPosition(position, direction);
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
