#ifndef SCENEITEM_HPP_
#define SCENEITEM_HPP_

#include <list>
#include <mutex>

#include "glload/gl_3_2.h"
#include <glm/glm.hpp>

#include "scene/collisiondetection/boundingvolume.hpp"

namespace scene {

	/**
	 * Abstract base class for a scene graph item; endnode.
	 *
	 * @author Michel Bouwmans
	 */
	class SceneItem {
		protected:
			glm::vec3 location;
			glm::mat4 modelMatrix;
			
			GLuint matrixUBO;
			GLuint shaderProgram;
			
			collisiondetection::BoundingVolume& bounds;
			
			mutable std::mutex locationMutex;
			mutable std::recursive_mutex matrixMutex;
		public:
			SceneItem(glm::vec3 initialLocation, collisiondetection::BoundingVolume& myBounds);
			
			/**
			 * Called every update-tick.
			 */
			virtual void update() = 0;

			/**
			 * Move the object.
			 */
			virtual void move();
			
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
			
			inline collisiondetection::BoundingVolume& getBounds();
			
			inline std::recursive_mutex& getMatrixMutex();
	};
	
	glm::vec3 SceneItem::getLocation() const {
		locationMutex.lock();
		glm::vec3 lastLocation = location;
		locationMutex.unlock();
		
		return lastLocation;
	}
	
	collisiondetection::BoundingVolume& SceneItem::getBounds() {
		return bounds;
	}
	
	std::recursive_mutex& SceneItem::getMatrixMutex() {
		return matrixMutex;
	}
}

#endif /* SCENEITEM_HPP_ */
