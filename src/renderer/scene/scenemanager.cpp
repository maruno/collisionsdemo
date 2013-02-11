#include "scenemanager.hpp"

#include <chrono>
#include <thread>
#include <mutex>

#include "glload/gl_3_2.h"
#include <GL/glfw.h>

#include "scenegroup.hpp"
#include "perspectivecamera.hpp"

#include "render/lightmanager.hpp"

#include "config/globals.hpp"

using namespace scene;

SceneManager::SceneManager()
: world(3, collisiondetection::AABB(std::make_tuple(glm::vec3(-10.0f, -10.0f, 0.0f), glm::vec3(10.0f, 10.0f, -50.0f)))),
running(false){
}

void SceneManager::startSceneLoop() {
	scene::PerspectiveCamera& camera = scene::PerspectiveCamera::getInstance();

	running = true;

	std::thread updateThread([this, &camera]() {
		while(running) {
			universalGravity.update();

			world.visitScene([](std::unique_ptr<SceneItem>& child) {
				child->update();

				child->move();
			});

			world.visitGroups([](SceneGroup& group) {
				if(group.constraints != nullptr) {
					auto it = group.childItems.begin();
					while(it != group.childItems.end()) {
						if(!group.constraints->intersects((*it)->getBounds())) {
							std::lock_guard<std::recursive_mutex> guard(group.rootNode->sceneMutex);

							group.rootNode->bubbleItem(std::move(*it));
							it = group.childItems.erase(it);
						} else {
							++it;
						}
					}
				}
			});

			std::this_thread::sleep_for(std::chrono::milliseconds((unsigned int)(1.0f/config::globals::updateRate)*1000));
		}
	});

	while(running) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		render::LightManager::getInstance().upload();
		camera.render(&world);

		glfwSwapBuffers();

		std::this_thread::sleep_for(std::chrono::milliseconds((unsigned int)(1.0f/config::globals::frameRate)*1000));
	}

	updateThread.join();
}

void SceneManager::stopSceneLoop() {
	running = false;
}

void SceneManager::addItem(std::unique_ptr<SceneItem> item) {
	GravitationalObject* gravObject = dynamic_cast<GravitationalObject*>(item.get());
	if(gravObject != nullptr) {
		universalGravity.addObject(gravObject);
	}

	std::lock_guard<std::recursive_mutex> guard(world.rootNode->sceneMutex);
	world.bubbleItem(std::move(item));
}
