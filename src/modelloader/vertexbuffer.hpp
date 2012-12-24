#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#include <tuple>

#include <glm/glm.hpp>

#include "glload/gl_3_2.h"

namespace modelloader {
	/**
	 * Wrapper class that represents a vertex buffer collection.
	 */
	class VertexBuffer {
	 private:
		GLuint vBO, iBO, nBO;
		GLuint vao;
		unsigned int numIndices;
		const std::tuple<glm::vec3, glm::vec3> extremes;

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
}

#endif // VERTEXBUFFER_HPP
