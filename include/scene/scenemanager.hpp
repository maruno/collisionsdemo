#ifndef SCENEMANAGER_HPP_
#define SCENEMANAGER_HPP_

#include <list>
#include <algorithm>
#include <mutex>

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
			std::list<SceneGroup*> worlds;
			std::list<PerspectiveCamera*> cameras;

			std::mutex updateMutex;
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
			 * Add a world.
			 *
			 * @param world New world.
			 */
			inline void addWorld(SceneGroup* world);

			/**
			 * Delete a world.
			 *
			 * @param world Old world.
			 */
			inline void delWorld(SceneGroup* world);

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
	};

	void SceneManager::addWorld(SceneGroup* world) {
		updateMutex.lock();
		worlds.push_back(world);
		updateMutex.unlock();
	}

	void SceneManager::delWorld(SceneGroup* world) {
		updateMutex.lock();
		std::remove(worlds.begin(), worlds.end(), world);
		updateMutex.unlock();
	}

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
