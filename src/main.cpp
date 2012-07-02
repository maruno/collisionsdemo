#include <memory>

#include <unistd.h>

#include "glload/gl_3_2.h"
#include "glload/gll.hpp"
#include <GL/glfw.h>

#include "config/globals.hpp"

#include "scene/scenemanager.hpp"
#include "scene/perspectivecamera.hpp"
#include "scene/scenegroup.hpp"

#include "sceneitems/genericplanet.hpp"

//HACK for stupid C-functions. Need to edit GLFW-source
scene::PerspectiveCamera* cameraPtr;

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
	scene::SceneGroup* world = new scene::SceneGroup;
	cameraPtr = new scene::PerspectiveCamera(world);
	scene::SceneManager sceneManager(cameraPtr, world);

	cameraPtr->changeCameraPosition(glm::vec3(20.0f, 0.0f, 150.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	cameraPtr->rescale(config::globals::initialWidth, config::globals::initialHeight);

	//TODO Hack support for lambda's in GLFW
	glfwSetWindowSizeCallback([](int width, int height) {
		cameraPtr->rescale(width, height);
	});

	glfwSetKeyCallback([](int key, int action) {
		switch(key) {
		case GLFW_KEY_UP:
			if(action == GLFW_PRESS) {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::UP_KEY_PRESSED);
			} else {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::NO_KEY_PRESSED);
			}
			break;
		case GLFW_KEY_DOWN:
			if(action == GLFW_PRESS) {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::DOWN_KEY_PRESSED);
			} else {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::NO_KEY_PRESSED);
			}
			break;
		case GLFW_KEY_LEFT:
			if(action == GLFW_PRESS) {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::LEFT_KEY_PRESSED);
			} else {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::NO_KEY_PRESSED);
			}
			break;
		case GLFW_KEY_RIGHT:
			if(action == GLFW_PRESS) {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::RIGHT_KEY_PRESSED);
			} else {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::NO_KEY_PRESSED);
			}
			break;
		case 'W':
			if(action == GLFW_PRESS) {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::W_KEY_PRESSED);
			} else {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::NO_KEY_PRESSED);
			}
			break;
		case 'A':
			if(action == GLFW_PRESS) {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::A_KEY_PRESSED);
			} else {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::NO_KEY_PRESSED);
			}
			break;
		case 'S':
			if(action == GLFW_PRESS) {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::S_KEY_PRESSED);
			} else {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::NO_KEY_PRESSED);
			}
			break;
		case 'D':
			if(action == GLFW_PRESS) {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::D_KEY_PRESSED);
			} else {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::NO_KEY_PRESSED);
			}
			break;
		case GLFW_KEY_PAGEUP:
			if(action == GLFW_PRESS) {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::PAGEUP_KEY_PRESSED);
			} else {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::NO_KEY_PRESSED);
			}
			break;
		case GLFW_KEY_PAGEDOWN:
			if(action == GLFW_PRESS) {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::PAGEDOWN_KEY_PRESSED);
			} else {
				cameraPtr->setKeyPressed(scene::PerspectiveCamera::NO_KEY_PRESSED);
			}
			break;
		}
	});

	scene::SceneItem* planeta = new sceneitems::GenericPlanet(glm::vec3(0.0f, 0.0f, 0.0f), 16.0f);
	scene::SceneItem* planetb = new sceneitems::GenericPlanet(glm::vec3(150.0f, 0.0f, -12.0f), 16.0f);
	scene::SceneItem* planetc = new sceneitems::GenericPlanet(glm::vec3(-35.0f, 45.0f, 0.0f), 20.0f);
	scene::SceneItem* planetd = new sceneitems::GenericPlanet(glm::vec3(0.0f, -45.0f, 30.0f), 5.0f);


	sceneManager.addItem(std::unique_ptr<scene::SceneItem>(planeta));
	sceneManager.addItem(std::unique_ptr<scene::SceneItem>(planetb));
	sceneManager.addItem(std::unique_ptr<scene::SceneItem>(planetc));
	sceneManager.addItem(std::unique_ptr<scene::SceneItem>(planetd));

	//Start main render loop
	sceneManager.startSceneLoop();
}
