#include "scene/world.hpp"

#include "glload/gl_3_2.h"

#include <glm/gtc/matrix_transform.hpp>

//Looking at the origin from an angle
const glm::mat4 scene::World::view = glm::lookAt<float>(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

glm::mat4 scene::World::viewProjection;

void scene::World::rescale(int width, int height) {
	glViewport(0, 0, width, height);
	
	//HOR+
	//The approximate field of view of a human eye is 95° out, 75° down, 60° in, 60° up
	glm::mat4 perspective = glm::infinitePerspective<float>(135.0f,static_cast<float>(width)/height, 1.0f);
	
	viewProjection = perspective * view;
}