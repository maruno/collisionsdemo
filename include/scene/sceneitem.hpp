#ifndef SCENEITEM_HPP_
#define SCENEITEM_HPP_

#include <list>
#include <glm/glm.hpp>

namespace scene {
	class SceneItem {
		private:
			glm::vec3 location;
		public:
			virtual void update() = 0;
			virtual void draw() = 0;

			glm::vec3 getLocation();
	};

}


#endif /* SCENEITEM_HPP_ */
