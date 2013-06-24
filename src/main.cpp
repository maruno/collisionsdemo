#include <memory>
#include <tuple>
#include <random>
#include <sstream>

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

#include "renderer/render/hud.hpp"

#include "asteroid.hpp"
#include "player.hpp"
#include "rocket.hpp"

//HACK for C access
scene::SceneManager* sceneManagerPtr;

unsigned int score;

//Update queue
dispatch_queue_t gcd_queue;
std::atomic<bool> gameOver;

int main(int argc, char** argv) {
	gameOver = false;

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

	//Initialise gltext
	render::HUD::getInstance().setViewPortSize(config::globals::initialWidth, config::globals::initialHeight);

	//Initialise GCD
	gcd_queue = dispatch_queue_create("Update Queue", DISPATCH_QUEUE_SERIAL);
	dispatch_source_t score_timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, gcd_queue);
	dispatch_source_set_timer(score_timer, DISPATCH_TIME_NOW, 1000000000, 500000);

	__block std::shared_ptr<render::HUDItem> scoreMsg(new render::HUDItem);
	scoreMsg->position = glm::uvec2{0, 0};
	scoreMsg->text = "score: 0";
	scoreMsg->pointSize = 24;

	render::HUD::getInstance().addItem(scoreMsg);

	score = 0;
	dispatch_source_set_event_handler(score_timer, ^{
		score += 50;

		render::HUD& hud = render::HUD::getInstance();
		hud.removeItem(scoreMsg);

		std::ostringstream newScore;
		newScore << "score: " << score;

		scoreMsg->text = newScore.str();
		hud.addItem(scoreMsg);
	});

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
		render::HUD::getInstance().setViewPortSize(width, height);
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
			case 'R':
				if (action == GLFW_PRESS) {
					Player::getInstance()->fireRocket();
				}
				break;
		}
	});

	//Add SceneItems
	std::mt19937 gen;
	std::uniform_real_distribution<float> locDis(-25, 25);
	std::uniform_int_distribution<unsigned int> massDis(1, 2);

	for(unsigned int i = 0; i < 150; ++i) {
		std::shared_ptr<Asteroid> asteroid(new Asteroid(glm::vec3{locDis(gen), locDis(gen), locDis(gen)}, massDis(gen)));
		sceneManager.addItem(asteroid);
	}

	sceneManager.addItem(Player::getInstance());

	dispatch_resume(score_timer);

	//Start main render loop
	sceneManager.startSceneLoop();

	glfwCloseWindow();

	//Cleanup GCD
	dispatch_semaphore_t sem = dispatch_semaphore_create(0);

	dispatch_source_set_cancel_handler(score_timer, ^{
		dispatch_semaphore_signal(sem);
	});

	dispatch_source_cancel(score_timer);
	dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);

	dispatch_release(sem);
	dispatch_release(score_timer);
	dispatch_release(gcd_queue);

	return EXIT_SUCCESS;
}
