#include <cassert>

#include <unistd.h>

#include "glload/gl_3_2.h"
#include "glload/gll.hpp"
#include <GL/glfw.h>

#include "config/globals.hpp"

#include "scene/perspectivecamera.hpp"
#include "scene/scenegroup.hpp"

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
	
	camera.rescale(config::globals::initialWidth, config::globals::initialHeight);

	/*TODO Hack support for lambda's in GLFW
	 * glfwSetWindowSizeCallback([](int width, int height) {
		scene::PerspectiveCamera::rescale(width, height);
	});*/
	
	/*//Define terrain
	sceneitems::Terrain terrain(scene::PerspectiveCamera::getMatrix(), 65, 75);
	terrainPtr = &terrain;


	glfwSetKeyCallback([](int keyId, int keyState) {
		switch(keyId) {
		case GLFW_KEY_LEFT:
			if(keyState == GLFW_PRESS) {
				terrainPtr->rotateLeft();
			} else {
				terrainPtr->stopRotation();
			}

			break;
		case GLFW_KEY_RIGHT:
			if(keyState == GLFW_PRESS) {
				terrainPtr->rotateRight();
			} else {
				terrainPtr->stopRotation();
			}

			break;
		}
	});

	//Main render loop
	while(true) {
		usleep(40000);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//HACK till we have a proper scene graph
		terrain.updateMatrix();
		terrain.render();

		glfwSwapBuffers();
	}*/

	glfwTerminate();
}
