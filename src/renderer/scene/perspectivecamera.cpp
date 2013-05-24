#include "perspectivecamera.hpp"

#include "glload/gl_3_2.h"

#include <glm/gtc/matrix_transform.hpp>

#include "scenegroup.hpp"

#include "config/globals.hpp"
#include "render/lightmanager.hpp"

using namespace scene;

std::unique_ptr<PerspectiveCamera> PerspectiveCamera::instance;

PerspectiveCamera::PerspectiveCamera() : direction{0.0f, 0.0f, -1.0f}, up{0.0f, 1.0f, 0.0f}, needsUpload{true}, lightID{0} {
	glGenBuffers(1, &uBO);
}

void PerspectiveCamera::rescale(int width, int height) {
	viewportWidth = width;
	viewportHeight = height;

	glViewport(0, 0, viewportWidth, viewportHeight);

	//HOR+
	//The approximate field of view of a human eye is 95° out, 75° down, 60° in, 60° up
	projection = glm::perspective<float>(60.0f, static_cast<float>(viewportWidth)/viewportHeight, 0.3f, 1000.0f);
}

void PerspectiveCamera::changeCameraPosition(glm::vec3 myPosition, glm::vec3 myDirection) {
	position = myPosition;
	direction = myDirection;

	//Update view matrix
	glm::vec3 lookAtCenter = direction;

	view = glm::lookAt<float>(position, lookAtCenter, up);

	//Camera light
	render::LightManager& lightManager = render::LightManager::getInstance();

	if(lightID) {
		//Update light
		lightManager.moveLightSource(lightID, position);
	} else {
		//Create light
		lightID = lightManager.addLightSource(position, 0.5f);
	}
}

void PerspectiveCamera::changeUpVector(glm::vec3 myUp) {
	up = myUp;

	//HOR+
	//The approximate field of view of a human eye is 95° out, 75° down, 60° in, 60° up
	projection = glm::perspective<float>(60.0f, static_cast<float>(viewportWidth)/viewportHeight, 0.3f, 1000.0f);
}

void PerspectiveCamera::render(SceneGroup* world) {
	upload();
	
	world->visitScene([this](std::shared_ptr<SceneItem> child) {
		child->render(view);
	});
}

void PerspectiveCamera::upload() {
	if(needsUpload) {
		glBindBuffer(GL_UNIFORM_BUFFER, uBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), &position, GL_DYNAMIC_DRAW);
		
		needsUpload = false;
	}
}