#include "scenemanager.hpp"

#include <chrono>
#include <thread>
#include <mutex>
#include <typeinfo>
#include <algorithm>

#include <dispatch/dispatch.h>

#include "glload/gl_3_2.h"
#include <GL/glfw.h>

#include "scenegroup.hpp"
#include "perspectivecamera.hpp"

#include "render/lightmanager.hpp"

#include "collisiondetection/collidable.hpp"
#include "collisiondetection/objectorientedboundingbox.hpp"

#include "config/globals.hpp"

using namespace scene;

extern dispatch_queue_t gcd_queue;

SceneManager::SceneManager()
: world(3, collisiondetection::AABB(std::make_tuple(glm::vec3(-25.0f, -25.0f, 10.0f), glm::vec3(10.0f, 10.0f, -50.0f)))),
running(false){
}

void SceneManager::startSceneLoop() {
	scene::PerspectiveCamera& camera = scene::PerspectiveCamera::getInstance();

	running = true;

	auto gcd_update_timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, gcd_queue);
	dispatch_source_set_timer(gcd_update_timer, DISPATCH_TIME_NOW, (uint64_t) (1.0f/config::globals::frameRate)*1000000000, 0);
	
	dispatch_source_set_event_handler(gcd_update_timer, ^{
		universalGravity.update();

		world.visitScene([](std::shared_ptr<SceneItem>& child) {
			child->update();

			child->buildModelMatrix();
		});

		world.visitGroups([](SceneGroup& group) {
			if(group.constraints != nullptr) {
				std::unique_lock<std::recursive_mutex> guard(group.rootNode->sceneMutex, std::defer_lock);
				std::vector<std::shared_ptr<SceneItem>> bubbleCandidates;

				auto it = group.childItems.begin();
				while(it != group.childItems.end()) {
					const collisiondetection::BoundingVolume& otherBounds = (*it)->getBounds();
					otherBounds.attachToItem((*it).get());

					if(!otherBounds.intersects(*(group.constraints))) {
						guard.lock();

						bubbleCandidates.push_back(*it);
						it = group.childItems.erase(it);
					} else {
						++it;
					}
				}

				for(std::shared_ptr<SceneItem>& candidate : bubbleCandidates) {
					group.rootNode->bubbleItem(candidate);
				}
			}
		});

		world.visitGroups([](SceneGroup& group) {
			for (std::shared_ptr<SceneItem>& child : group.childItems) {
				auto collidable = std::dynamic_pointer_cast<collisiondetection::Collidable>(child);

				if(collidable.get() != nullptr) {
					std::function<void(const SceneGroup&)> collisionCheck = [collidable, child](const SceneGroup& otherGroup) {
						for(const std::shared_ptr<SceneItem>& other : otherGroup.childItems) {
							if(child.get() != other.get()) {
								const collisiondetection::ObjectOrientedBoundingBox collidableBounds = child->getBounds();
								collidableBounds.attachToItem(child.get());

								const collisiondetection::ObjectOrientedBoundingBox otherBounds = other->getBounds();
								otherBounds.attachToItem(other.get());

								if(collidableBounds.intersects(otherBounds)) {
									collidable->handleCollision(*other);
								}
							}
						}
					};

					collisionCheck(group);

					if(group.childGroups != nullptr) {
						//Check underlying groups for lower edge cases
						group.visitGroups(collisionCheck);
					}

					//Check with parents for upper edge cases
					group.visitParentGroups(collisionCheck);
				}
			}
		});
	});

	dispatch_resume(gcd_update_timer);

	while(running) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		render::LightManager::getInstance().upload();
		camera.render(&world);

		glfwSwapBuffers();

		std::this_thread::sleep_for(std::chrono::milliseconds((unsigned int)(1.0f/config::globals::frameRate)*1000));
	}

	//Clean up GCD
	auto sem = dispatch_semaphore_create(0);

	dispatch_source_set_cancel_handler(gcd_update_timer, ^{
		dispatch_semaphore_signal(sem);
	});

	dispatch_source_cancel(gcd_update_timer);
	dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);

	dispatch_release(sem);
	dispatch_release(gcd_update_timer);
	dispatch_release(gcd_queue);
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
