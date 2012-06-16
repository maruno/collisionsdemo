#ifndef SCENEMANAGER_HPP_
#define SCENEMANAGER_HPP_

#include <list>
#include <algorithm>
#include <mutex>

namespace scene {
	class SceneGroup;
	class PerspectiveCamera;

	class SceneManager {
		private:
			std::list<SceneGroup*> worlds;
			std::list<PerspectiveCamera*> cameras;
			
			std::mutex updateMutex;
			std::mutex renderMutex;
		public:
			SceneManager(PerspectiveCamera* primaryCamera, SceneGroup* primaryWorld);
			
			void startSceneLoop();
			
			inline void addWorld(SceneGroup* world);
			inline void delWorld(SceneGroup* world);

			inline void addCamera(PerspectiveCamera* camera);
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
