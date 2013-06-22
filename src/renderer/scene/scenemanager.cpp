#include "scenemanager.hpp"

#include <chrono>
#include <thread>
#include <mutex>
#include <typeinfo>
#include <algorithm>
#include <chrono>

#include <dispatch/dispatch.h>

#include "glload/gl_3_2.h"
#include <GL/glfw.h>

#include "scenegroup.hpp"
#include "perspectivecamera.hpp"

#include "render/lightmanager.hpp"
#include "render/hud.hpp"

#include "collisiondetection/collidable.hpp"
#include "collisiondetection/objectorientedboundingbox.hpp"

#include "config/globals.hpp"

#include "player.hpp"

using namespace scene;

extern std::atomic<bool> gameOver;
extern dispatch_queue_t gcd_queue;
std::chrono::milliseconds time_since_last_update;

SceneManager::SceneManager()
: world(3, collisiondetection::AABB(std::make_tuple(glm::vec3(-50.0f, -50.0f, 50.0f), glm::vec3(50.0f, 50.0f, -50.0f)))),
running(false){
}

void SceneManager::startSceneLoop() {
	scene::PerspectiveCamera& camera = scene::PerspectiveCamera::getInstance();

	running = true;

	__block std::chrono::time_point<std::chrono::steady_clock> previousTime = std::chrono::steady_clock::now();

	auto gcd_update_timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, gcd_queue);
	dispatch_source_set_timer(gcd_update_timer, DISPATCH_TIME_NOW, (uint64_t) (1.0f/config::globals::frameRate)*1000000000, 0);
	
	dispatch_source_set_event_handler(gcd_update_timer, ^{
		std::chrono::time_point<std::chrono::steady_clock> newTime = std::chrono::steady_clock::now();

		time_since_last_update = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - previousTime);
		previousTime = newTime;

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
						if(!guard.owns_lock()) {
							guard.lock();
						}

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

		std::vector<std::pair<collisiondetection::Collidable*, collisiondetection::Collidable*>> dontCollide;
		world.visitGroups([&dontCollide](SceneGroup& group) {
			for (std::shared_ptr<SceneItem>& child : group.childItems) {
				auto collidable = std::dynamic_pointer_cast<collisiondetection::Collidable>(child);

				if(collidable.get() != nullptr) {
					std::function<void(const SceneGroup&)> collisionCheck = [collidable, child, &dontCollide](const SceneGroup& otherGroup) {
						for(const std::shared_ptr<SceneItem>& other : otherGroup.childItems) {
							if(child.get() != other.get()) {
								const collisiondetection::ObjectOrientedBoundingBox collidableBounds = child->getBounds();
								collidableBounds.attachToItem(child.get());

								const collisiondetection::ObjectOrientedBoundingBox otherBounds = other->getBounds();
								otherBounds.attachToItem(other.get());

								if(collidableBounds.intersects(otherBounds)) {
									auto otherCollidable = std::dynamic_pointer_cast<collisiondetection::Collidable>(other);
									if(otherCollidable.get() != nullptr) {
										//Don't collide same objects twice
										if(std::find(dontCollide.begin(), dontCollide.end(), std::make_pair(collidable.get(), otherCollidable.get())) == dontCollide.end()) {
											dontCollide.push_back(std::make_pair(otherCollidable.get(), collidable.get()));

											//Always prefer Player
											if (typeid(*otherCollidable) == typeid(Player)) {
												otherCollidable->handleCollision(*child);
											} else {
												collidable->handleCollision(*other);
											}
										}
									} else {
										collidable->handleCollision(*other);
									}
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

		if (gameOver) {
			std::shared_ptr<render::HUDItem> gameOverMsg(new render::HUDItem);
			gameOverMsg->pointSize = 96;
			gameOverMsg->text = "GAME OVER";
			gameOverMsg->position = glm::uvec2{200, 384};

			render::HUD::getInstance().addItem(gameOverMsg);

			dispatch_source_cancel(gcd_update_timer);
			auto sem = dispatch_semaphore_create(0);

			dispatch_source_set_cancel_handler(gcd_update_timer, ^{
				dispatch_semaphore_signal(sem);
			});

			dispatch_source_cancel(gcd_update_timer);
			dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);

			dispatch_release(sem);
			dispatch_release(gcd_update_timer);
		}
	});

	dispatch_resume(gcd_update_timer);

	while(running) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		render::LightManager::getInstance().upload();
		camera.render(&world);
		render::HUD::getInstance().render();

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

void SceneManager::removeItem(std::shared_ptr<SceneItem> item) {
	GravitationalObject* gravObject = dynamic_cast<GravitationalObject*>(item.get());
	if(gravObject != nullptr) {
		universalGravity.delObject(gravObject);
	}

	std::lock_guard<std::recursive_mutex> guard(world.rootNode->sceneMutex);
	world.visitGroups([item](SceneGroup& group){
		auto it = std::find(group.childItems.begin(), group.childItems.end(), item);

		if(it != group.childItems.end()) {
			group.childItems.erase(it);

			return;
		}
	});
}
