#ifndef SCENEITEM_HPP_
#define SCENEITEM_HPP_

#include <list>

#include "glload/gl_3_2.h"
#include <glm/glm.hpp>

namespace scene {

	/**
	 * Abstract base class for a scene graph item; endnode.
	 *
	 * @author Michel Bouwmans
	 */
	class SceneItem {
		protected:
			glm::vec3 location;
			GLuint matrixUBO;
			GLuint shaderProgram;
			glm::mat4 modelMatrix;
		public:
			SceneItem(glm::vec3 initialLocation);
			
			/**
			 * Called every update-tick.
			 */
			virtual void update() = 0;

			/**
			 * Called every render-tick.
			 */
			virtual void render(glm::mat4& parentMatrix) const = 0;

			/**
			 * Return the location of this item.
			 *
			 * @return Item location.
			 */
			inline glm::vec3 getLocation() const;
	};

	glm::vec3 SceneItem::getLocation() const {
		return location;
	}
}


#endif /* SCENEITEM_HPP_ */
