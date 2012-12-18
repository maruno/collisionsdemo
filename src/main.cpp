#include <memory>
#include <tuple>

#include <unistd.h>

#include "glload/gl_3_2.h"
#include "glload/gll.hpp"
#include <GL/glfw.h>

#include <glm/glm.hpp>

#include "config/globals.hpp"

#include "scene/scenemanager.hpp"
#include "scene/perspectivecamera.hpp"
#include "scene/scenegroup.hpp"

#include "scene/collisiondetection/axisalignedboundingcuboid.hpp"

int main(int argc, char** argv) {
	glfwInit();

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, config::globals::multiSamples);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwOpenWindow(config::globals::initialWidth, config::globals::initialHeight, 8, 8, 8, 8, 24, 24, GLFW_WINDOW);
	glfwSetWindowTitle("Awesome planetary simulation demo");

	//Load OpenGL functions
	if(glload::LoadFunctions() == glload::LS_LOAD_FAILED) {
		return 1;
	}

	//Enable depth test
	glEnable(GL_DEPTH_TEST);

	//Backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//Set clear colour to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Define world
	scene::PerspectiveCamera& camera = scene::PerspectiveCamera::getInstance();
	scene::SceneManager sceneManager;

	camera.changeCameraPosition(glm::vec3(20.0f, 0.0f, 150.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	camera.rescale(config::globals::initialWidth, config::globals::initialHeight);

	glfwSetWindowSizeCallback([](int width, int height) {
		scene::PerspectiveCamera::getInstance().rescale(width, height);
	});

	glfwSetKeyCallback([](int key, int action) {
		switch(key) {
			//TODO Switch keys for control
		}
	});

	//TODO SceneItems

	//Start main render loop
	sceneManager.startSceneLoop();
}
