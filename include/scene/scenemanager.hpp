#ifndef SCENEMANAGER_HPP_
#define SCENEMANAGER_HPP_

#include <list>
#include <algorithm>
#include <mutex>
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
			std::list<PerspectiveCamera*> cameras;

			UniversalGravitation universalGravity;

			std::mutex renderMutex;
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
			 * Add a camera.
			 *
			 * @param camera New camera.
			 */
			inline void addCamera(PerspectiveCamera* camera);

			/**
			 * Delete a camera.
			 *
			 * @param camera Old camera.
			 */
			inline void delCamera(PerspectiveCamera* camera);

			/**
			 * Add an item under scene graph control.
			 *
			 * @param item Unique pointer to the @ref SceneItem. The scene graph takes ownership.
			 */
			void addItem(std::unique_ptr<SceneItem> item);
	};

	void SceneManager::addCamera(PerspectiveCamera* camera) {
		renderMutex.lock();
		cameras.push_back(camera);
		renderMutex.unlock();
	}

	void SceneManager::delCamera(PerspectiveCamera* camera) {
		renderMutex.lock();
		std::remove(cameras.begin(), cameras.end(), camera);
		renderMutex.unlock();
	}
}

#endif /* SCENEMANAGER_HPP_ */
