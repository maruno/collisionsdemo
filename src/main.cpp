#include <cassert>
#include <memory>
#include <thread>

#include <unistd.h>

#include "glload/gl_3_2.h"
#include "glload/gll.hpp"
#include <GL/glfw.h>

#include "config/globals.hpp"

#include "scene/scenemanager.hpp"
#include "scene/perspectivecamera.hpp"
#include "scene/scenegroup.hpp"

#include "scene/universalgravitation.hpp"

#include "sceneitems/genericplanet.hpp"

//HACK for stupid C-functions. Need to edit GLFW-source
//sceneitems::Terrain* terrainPtr;

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

	//Set clear colour to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Define world
	scene::SceneGroup world;
	scene::PerspectiveCamera camera(&world);
	
	camera.changeCameraPosition(glm::vec3(500.0f, 0.0f, 1000.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	camera.rescale(config::globals::initialWidth, config::globals::initialHeight);
	
	/*TODO Hack support for lambda's in GLFW
	 * glfwSetWindowSizeCallback([](int width, int height) {
		scene::PerspectiveCamera::rescale(width, height);
	});*/
	
	//TODO Move UniversalGravitation into scene graph!
	scene::UniversalGravitation universalGravity;
	
	scene::GravitationalObject* planeta = new sceneitems::GenericPlanet(glm::vec3(0.0f, 0.0f, 0.0f), 20.0f);
	scene::GravitationalObject* planetb = new sceneitems::GenericPlanet(glm::vec3(1500.0f, 0.0f, 0.0f), 8.0f);
	
	universalGravity.addObject(planeta);
	universalGravity.addObject(planetb);
	
	world.addItem(std::unique_ptr<scene::SceneItem>(planeta));
	world.addItem(std::unique_ptr<scene::SceneItem>(planetb));
	
	//Start main render loop
	scene::SceneManager sceneManager(&camera, &world);
	sceneManager.startSceneLoop();
}
