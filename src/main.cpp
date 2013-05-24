#include <memory>
#include <tuple>
#include <random>

#include "glload/gl_3_2.h"
#include "glload/gll.hpp"
#include <GL/glfw.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_AVX
#define GLM_MESSAGES
#include <glm/glm.hpp>

#include <dispatch/dispatch.h>

#include "config/globals.hpp"

#include "renderer/scene/scenemanager.hpp"
#include "renderer/scene/perspectivecamera.hpp"

#include "asteroid.hpp"
#include "player.hpp"

//HACK for C access
scene::SceneManager* sceneManagerPtr;

//Update queue
dispatch_queue_t gcd_queue;

int main(int argc, char** argv) {
	glfwInit();

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, config::globals::multiSamples);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwOpenWindow(config::globals::initialWidth, config::globals::initialHeight, 8, 8, 8, 8, 8, 8, GLFW_WINDOW);
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

	//Initialize GCD
	gcd_queue = dispatch_queue_create("Update Queue", DISPATCH_QUEUE_SERIAL);

	//Define world
	scene::PerspectiveCamera::getInstance().rescale(config::globals::initialWidth, config::globals::initialHeight);
	scene::SceneManager sceneManager;

	//HACK for C access
	sceneManagerPtr =  &sceneManager;

	glfwSetWindowCloseCallback([](){
		sceneManagerPtr->stopSceneLoop();

		return GL_FALSE;
	});

	glfwSetWindowSizeCallback([](int width, int height) {
		scene::PerspectiveCamera::getInstance().rescale(width, height);
	});

	glfwSetKeyCallback([](int key, int action) {
		switch(key) {
			case 'A':
				Player::getInstance()->yaw(action? 1 : 0);
				break;
			case 'D':
				Player::getInstance()->yaw(action? -1 : 0);
				break;
			case 'W':
				Player::getInstance()->pitch(action? 1 : 0);
				break;
			case 'S':
				Player::getInstance()->pitch(action? -1 : 0);
				break;
		}
	});

	//Add SceneItems
	std::mt19937 gen;
	std::uniform_real_distribution<float> locDis(-40, 40);
	std::uniform_int_distribution<unsigned int> massDis(10, 50);

	for(unsigned int i = 0; i < 50; ++i) {
		scene::SceneItem* asteroid = new Asteroid(glm::vec3{locDis(gen), locDis(gen), locDis(gen)}, massDis(gen));
		sceneManager.addItem(std::shared_ptr<scene::SceneItem>(asteroid));
	}

	sceneManager.addItem(Player::getInstance());

	//Start main render loop
	sceneManager.startSceneLoop();

	glfwCloseWindow();

	return EXIT_SUCCESS;
}
