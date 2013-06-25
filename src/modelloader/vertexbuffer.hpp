#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#include <tuple>
#include <memory>
#include <vector>
#include <exception>

#include <glm/glm.hpp>

#include "glload/gl_3_2.h"

#include "collisiondetection/objectorientedboundingbox.hpp"

namespace modelloader {
	class DataNotAvailable : public std::runtime_error {
	 public:
		DataNotAvailable() : std::runtime_error("Requested data not available for this 3D-model") {}
	};

	/**
	 * Wrapper class that represents a vertex buffer collection.
	 */
	class VertexBuffer {
	 friend class VertexBufferFactory;
	 private:
		GLuint vBO, iBO, nBO;
		GLuint vao;
		unsigned int numIndices;
		const std::tuple<glm::vec3, glm::vec3> extremes;
		std::unique_ptr<collisiondetection::ObjectOrientedBoundingBox> bounds;
		std::unique_ptr<std::vector<glm::vec3>> vertexData;
		std::unique_ptr<std::vector<glm::vec3>> normalsData;
		std::unique_ptr<std::vector<glm::uvec3>> indicesData;

	 public:
		/**
		 * Constructor.
		 *
		 * @param myVBO 'Vertex'/position Buffer Object of this vertex buffer collection.
		 * @param myIBO Indice Buffer Object of this vertex buffer collection.
		 * @param myNumIndices Number of indices associated with this vertex buffer collection.
		 */
		VertexBuffer(GLuint myVBO, GLuint myIBO, GLuint myNBO, unsigned int myNumIndices, std::tuple<glm::vec3, glm::vec3> myExtremes);

		/**
		 * Move constructor.
		 */
		VertexBuffer(VertexBuffer&& other);

		/**
		 * Request the 'Vertex'/position Buffer Object.
		 *
		 * @return 'Vertex'/position Buffer Object identifier.
		 */
		inline const GLuint getVBO() const;

		/**
		 * Request the Normal Buffer Object.
		 *
		 * @return Normal Buffer Object identifier.
		 */
		inline const GLuint getNBO() const;

		/**
		 * Request the Indice Buffer Object.
		 *
		 * @return Indice Buffer Object identifier.
		 */
		inline const GLuint getIBO() const;

		/**
		 * Request the number of indices.
		 *
		 * @return Number of indices.
		 */
		inline const unsigned int getNumIndices() const;

		/**
		 * Request the extremes of the 3D-model
		 *
		 * @return Extremes of the 3D-model
		 */
		inline const std::tuple<glm::vec3, glm::vec3>& getExtremes() const;

		/**
		 * Request the bounds of the 3D-model
		 *
		 * @return ObjectOrientedBoundingBox attached to the 3D-model
		 */
		inline const collisiondetection::ObjectOrientedBoundingBox& getBounds() const;

		/**
		 * Request the vertex data of the 3D-model
		 *
		 * \return Vertex data of this 3D-model
		 * \throw DataNotAvailable Requested vertex data was not loaded.
		 */
		inline const std::vector<glm::vec3>& getVertexData() const;

		/**
		 * Request the normal data of the 3D-model
		 *
		 * \return Normal data of this 3D-model
		 * \throw DataNotAvailable Requested normal data was not loaded.
		 */
		inline const std::vector<glm::vec3>& getNormalData() const;

		/**
		 * Request the indices data of the 3D-model
		 *
		 * \return Indices data of this 3D-model
		 * \throw DataNotAvailable Requested indices data was not loaded.
		 */
		inline const std::vector<glm::uvec3>& getIndicesData() const;

		/**
		 * Bind the buffers from this vertex buffer collection for use.
		 */
		void bindBuffers() const;
	};

	const GLuint VertexBuffer::getVBO() const {
		return vBO;
	}

	const GLuint VertexBuffer::getNBO() const {
		return nBO;
	}

	const GLuint VertexBuffer::getIBO() const {
		return iBO;
	}

	const unsigned int VertexBuffer::getNumIndices() const {
		return numIndices;
	}

	const std::tuple<glm::vec3, glm::vec3>& VertexBuffer::getExtremes() const {
		return extremes;
	}

	const collisiondetection::ObjectOrientedBoundingBox& VertexBuffer::getBounds() const {
		return *bounds;
	}

	const std::vector<glm::vec3>& VertexBuffer::getVertexData() const {
		if (vertexData.get() == nullptr) {
			throw DataNotAvailable();
		}

		return *vertexData;
	}

	const std::vector<glm::vec3>& VertexBuffer::getNormalData() const {
		if (normalsData.get() == nullptr) {
			throw DataNotAvailable();
		}

		return *normalsData;
	}

	const std::vector<glm::uvec3>& VertexBuffer::getIndicesData() const {
		if (indicesData.get() == nullptr) {
			throw DataNotAvailable();
		}

		return *indicesData;
	}
}

#endif // VERTEXBUFFER_HPP
