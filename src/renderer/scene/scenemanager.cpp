#include "scenemanager.hpp"

#include <chrono>
#include <thread>
#include <mutex>
#include <typeinfo>

#include "glload/gl_3_2.h"
#include <GL/glfw.h>

#include "scenegroup.hpp"
#include "perspectivecamera.hpp"

#include "render/lightmanager.hpp"

#include "collisiondetection/collidable.hpp"
#include "collisiondetection/objectorientedboundingbox.hpp"

#include "config/globals.hpp"

using namespace scene;

SceneManager::SceneManager()
: world(3, collisiondetection::AABB(std::make_tuple(glm::vec3(-10.0f, -10.0f, 10.0f), glm::vec3(10.0f, 10.0f, -50.0f)))),
running(false){
}

void SceneManager::startSceneLoop() {
	scene::PerspectiveCamera& camera = scene::PerspectiveCamera::getInstance();

	running = true;

	std::thread updateThread([this, &camera]() {
		while(running) {
			universalGravity.update();

			world.visitScene([](std::shared_ptr<SceneItem> child) {
				child->update();

				child->buildModelMatrix();
			});

			world.visitGroups([](SceneGroup& group) {
				for (std::shared_ptr<SceneItem>& child : group.childItems) {
					auto collidable = std::dynamic_pointer_cast<collisiondetection::Collidable>(child);

					if(collidable.get() != nullptr) {
						for (std::shared_ptr<SceneItem>& other : group.childItems) {
							if(other != child) {
								//TODO Collidables should always have a copy of their bounds

								const collisiondetection::BoundingVolume& collidableBounds = child->getBounds();
								collidableBounds.attachToItem(child.get());

								const collisiondetection::BoundingVolume& otherBounds = other->getBounds();
								otherBounds.attachToItem(other.get());

								if(collidableBounds.intersects(otherBounds)) {
									collidable->handleCollision(*other);
								}
							}
						}

						if(group.childGroups != nullptr) {
							//Check underlying groups for edge cases
							for(SceneGroup& childGroup : *group.childGroups) {
								for (std::shared_ptr<SceneItem>& other : childGroup.childItems) {
									if(other != child) {
										//TODO Collidables should always have a copy of their bounds

										const collisiondetection::BoundingVolume& collidableBounds = child->getBounds();
										collidableBounds.attachToItem(child.get());

										const collisiondetection::BoundingVolume& otherBounds = other->getBounds();
										otherBounds.attachToItem(other.get());

										if(collidableBounds.intersects(otherBounds)) {
											collidable->handleCollision(*other);
										}
									}
								}
							}
						}
					}
				}
			});

			world.visitGroups([](SceneGroup& group) {
				if(group.constraints != nullptr) {
					auto it = group.childItems.begin();
					while(it != group.childItems.end()) {
						const collisiondetection::BoundingVolume& otherBounds = (*it)->getBounds();
						otherBounds.attachToItem((*it).get());

						if(!otherBounds.intersects(*(group.constraints))) {
							std::lock_guard<std::recursive_mutex> guard(group.rootNode->sceneMutex);

							group.rootNode->bubbleItem(*it);
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

void SceneManager::addItem(std::shared_ptr<SceneItem> item) {
	GravitationalObject* gravObject = dynamic_cast<GravitationalObject*>(item.get());
	if(gravObject != nullptr) {
		universalGravity.addObject(gravObject);
	}

	std::lock_guard<std::recursive_mutex> guard(world.rootNode->sceneMutex);
	world.bubbleItem(item);
}
