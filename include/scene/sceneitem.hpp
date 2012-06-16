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

			inline glm::vec3 getLocation();
	};

	glm::vec3 SceneItem::getLocation() {
		return location;
	}
}


#endif /* SCENEITEM_HPP_ */
