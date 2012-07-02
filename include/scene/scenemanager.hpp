#ifndef SCENEMANAGER_HPP_
#define SCENEMANAGER_HPP_

#include <list>
#include <algorithm>
#include <memory>

#include "scene/universalgravitation.hpp"
#include "scene/sceneitem.hpp"

namespace scene {
	class SceneGroup;
	class PerspectiveCamera;

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
			SceneGroup* world;
			PerspectiveCamera* camera;

			UniversalGravitation universalGravity;

		public:
			/**
			 * Constructor.
			 *
			 * @param primaryCamera Camera for primaryWorld.
			 * @param primaryWorld Primary world for this scene graph.
			 */
			SceneManager(PerspectiveCamera* primaryCamera, SceneGroup* primaryWorld);

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
