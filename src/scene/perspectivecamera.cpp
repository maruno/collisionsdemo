#include "scene/perspectivecamera.hpp"

#include "glload/gl_3_2.h"

#include <glm/gtc/matrix_transform.hpp>

#include "scene/scenegroup.hpp"

using namespace scene;

glm::vec3 PerspectiveCamera::up(0.0f, 1.0f, 0.0f);

PerspectiveCamera::PerspectiveCamera(SceneGroup* myWorld) : world(myWorld) {
}

void PerspectiveCamera::rescale(int width, int height) {
	glViewport(0, 0, width, height);

	//HOR+
	//The approximate field of view of a human eye is 95° out, 75° down, 60° in, 60° up
	glm::mat4 perspective = glm::perspective<float>(60.0f, static_cast<float>(width)/height, 0.3f, 10.0f);

	viewProjection = perspective * view;
}

void PerspectiveCamera::changeCameraPosition(glm::vec3 position, glm::vec3 direction) {
	glm::vec3 lookAtCenter =  position + direction;

	view = glm::lookAt<float>(position, lookAtCenter, glm::vec3(0.0f, 1.0f, 0.0f));
}

void PerspectiveCamera::render() {
	world->renderScene();
}
