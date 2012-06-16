#ifndef SCENEITEM_HPP_
#define SCENEITEM_HPP_

#include <list>
#include <glm/glm.hpp>

namespace scene {
	
	/**
	 * Abstract base class for a scene graph item; endnode.
	 * 
	 * @author Michel Bouwmans
	 */
	class SceneItem {
		private:
			glm::vec3 location;
		public:
			/**
			 * Called every update-tick.
			 */
			virtual void update() = 0;
			
			/**
			 * Called every render-tick.
			 */
			virtual void draw() = 0;

			/**
			 * Return the location of this item.
			 * 
			 * @return Item location.
			 */
			inline glm::vec3 getLocation();
	};

	glm::vec3 SceneItem::getLocation() {
		return location;
	}
}


#endif /* SCENEITEM_HPP_ */
