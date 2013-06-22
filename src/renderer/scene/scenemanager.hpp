#ifndef SCENEMANAGER_HPP_
#define SCENEMANAGER_HPP_

#include <list>
#include <algorithm>
#include <memory>
#include <atomic>

#include "renderer/scene/universalgravitation.hpp"
#include "renderer/scene/sceneitem.hpp"

#include "renderer/scene/scenegroup.hpp"

namespace scene {
	/**
	 * Management class for the scene graph.
	 *
	 * This class manages all main operations on the the scene graph.
	 * It's main goal is to handle the main scene loop.
	 *
	 * @author Michel Bouwmans
	 */
	class SceneManager {
	 private:
		SceneGroup world;

		UniversalGravitation universalGravity;
		std::atomic<bool> running;

	 public:
		/**
		 * Constructor.
		 */
		SceneManager();

		/**
		 * Start the multithreaded main scene loop. Framebuffer should be ready.
		 */
		void startSceneLoop();

		/**
		 * Stop the multithreaded main scene loop and return startSceneLoop.
		 */
		void stopSceneLoop();

		/**
		 * Add an item under scene graph control.
		 *
		 * @param item Shared pointer to the @ref SceneItem. The scene graph takes ownership.
		 */
		void addItem(std::shared_ptr<SceneItem> item);

		/**
		 * Remove an item under scene graph control.
		 *
		 * @param item Shared pointer to the @ref SceneItem.
		 */
		void removeItem(std::shared_ptr<SceneItem> item);
	};
}

#endif /* SCENEMANAGER_HPP_ */
