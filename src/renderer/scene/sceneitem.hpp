#ifndef SCENEITEM_HPP_
#define SCENEITEM_HPP_

#include <list>
#include <mutex>

#include "glload/gl_3_2.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "collisiondetection/boundingvolume.hpp"

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
		glm::vec3 scale;
		glm::quat rotation;

		glm::mat4 modelMatrix;

		GLuint matrixUBO;
		GLuint shaderProgram;

		const modelloader::VertexBuffer& vBuffers;

		mutable std::mutex locationMutex;
		mutable std::recursive_mutex matrixMutex;
	 public:
		/**
		 * Constructor.
		 *
		 * @param initialLocation The initial location for this item.
		 * @param objectName Name of the obj-file without extension.
		 * @param myBounds Bounding volume for this item.
		 */
		SceneItem(glm::vec3 initialLocation, std::string objectName);

		/**
		 * Called every update-tick.
		 */
		virtual void update() = 0;

		/**
		 * Rebuild the model matrix.
		 */
		virtual void buildModelMatrix();

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

		/**
		 * Get the model matrix for this item.
		 *
		 * \return Model matrix for this item.
		 */
		inline const glm::mat4& getModelMatrix() const;

		/**
		 * Get the rotation for this item.
		 *
		 * \return Rotation quaternion for this item.
		 */
		inline const glm::quat& getRotation() const;

		/**
		 * Get the scale for this item.
		 *
		 * \return Scale vec3 for this item.
		 */
		inline const glm::vec3& getScale() const;

		virtual ~SceneItem() = default;
	};

	glm::vec3 SceneItem::getLocation() const {
		std::unique_lock<std::mutex> lock(locationMutex);

		return location;
	}

	const collisiondetection::BoundingVolume& SceneItem::getBounds() const {
		return vBuffers.getBounds();
	}

	std::recursive_mutex& SceneItem::getMatrixMutex() {
		return matrixMutex;
	}

	const glm::mat4& SceneItem::getModelMatrix() const {
		return modelMatrix;
	}

	const glm::quat& SceneItem::getRotation() const {
		return rotation;
	}

	const glm::vec3& SceneItem::getScale() const {
		return scale;
	}
}

#endif /* SCENEITEM_HPP_ */
