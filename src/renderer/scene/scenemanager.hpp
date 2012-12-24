#ifndef SCENEMANAGER_HPP_
#define SCENEMANAGER_HPP_

#include <list>
#include <algorithm>
#include <memory>

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
		 * Add an item under scene graph control.
		 *
		 * @param item Unique pointer to the @ref SceneItem. The scene graph takes ownership.
		 */
		void addItem(std::unique_ptr<SceneItem> item);
	};
}

#endif /* SCENEMANAGER_HPP_ */
