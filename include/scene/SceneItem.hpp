/*
 * SceneItem.hpp
 *
 *      Author: Marcel Veltman
 */

#ifndef SCENEITEM_HPP_
#define SCENEITEM_HPP_

#include <list>
#include <glm/glm.hpp>

namespace scene {

	class SceneItem {
		private:
			std::list<SceneItem*>* children;
			glm::vec3 location;
		protected:
			virtual void update() = 0;
			virtual void updateScene();
			virtual void draw() = 0;
			virtual void drawScene();
		public:
			SceneItem(SceneItem* parent = nullptr);
			virtual ~SceneItem();
			glm::vec3 getLocation();
	};

}


#endif /* SCENEITEM_HPP_ */
