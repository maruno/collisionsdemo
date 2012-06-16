#include "scene/scenemanager.hpp"

#include <chrono>
#include <thread>

#include "glload/gl_3_2.h"
#include <GL/glfw.h>

#include "scene/scenegroup.hpp"
#include "scene/perspectivecamera.hpp"

#include "config/globals.hpp"

using namespace scene;

SceneManager::SceneManager(PerspectiveCamera* primaryCamera, SceneGroup* primaryWorld)
	: cameras({primaryCamera}), worlds({primaryWorld}) {
}

void SceneManager::startSceneLoop() {
	std::thread updateThread([this]() {
		updateMutex.lock();
		std::for_each(worlds.begin(), worlds.end(), [](SceneGroup* world) {
			world->updateScene();
		});
		updateMutex.unlock();
		
		std::this_thread::sleep_for(std::chrono::milliseconds((unsigned int)(1.0f/config::globals::updateRate)*1000));
	});

	std::thread renderThread([this]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		renderMutex.lock();
		std::for_each(cameras.begin(), cameras.end(), [](PerspectiveCamera* camera) {
			camera->render();
		});
		renderMutex.unlock();
		
		glfwSwapBuffers();
		
		std::this_thread::sleep_for(std::chrono::milliseconds((unsigned int)(1.0f/config::globals::frameRate)*1000));
	});
}
