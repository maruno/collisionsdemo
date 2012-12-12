#ifndef SCENEITEM_HPP_
#define SCENEITEM_HPP_

#include <list>
#include <mutex>

#include "glload/gl_3_2.h"
#include <glm/glm.hpp>

#include "scene/collisiondetection/boundingvolume.hpp"

#include "modelloader/vertexbuffer.hpp"

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

			const modelloader::VertexBuffer& vBuffers;
			const collisiondetection::BoundingVolume& bounds;

			mutable std::mutex locationMutex;
			mutable std::recursive_mutex matrixMutex;
		public:
			/**
			 * Constructor.
			 *
			 * @param initialLocation The initial location for this item.
			 * @param myBounds Bounding volume for this item.
			 */
			SceneItem(glm::vec3 initialLocation, std::string objectName, scene::collisiondetection::BoundingVolume& myBounds);

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

			/**
			 * Get the bounding volume for this item.
			 *
			 * @return Bounding volume for this item.
			 */
			inline const collisiondetection::BoundingVolume& getBounds() const;

			/**
			 * Get the matrix mutex for this item.
			 *
			 * @return Matrix mutex for this item.
			 */
			inline std::recursive_mutex& getMatrixMutex();
	};

	glm::vec3 SceneItem::getLocation() const {
		locationMutex.lock();
		glm::vec3 lastLocation = location;
		locationMutex.unlock();

		return lastLocation;
	}

	const collisiondetection::BoundingVolume& SceneItem::getBounds() const {
		return bounds;
	}

	std::recursive_mutex& SceneItem::getMatrixMutex() {
		return matrixMutex;
	}
}

#endif /* SCENEITEM_HPP_ */
