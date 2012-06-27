#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#include "glload/gl_3_2.h"

namespace modelloader {
	/**
	 * Wrapper class that represents a vertex buffer collection.
	 *
	 * TODO Convert to VAO structure!?
	 */
	class VertexBuffer {
		private:
			GLuint vBO, iBO, nBO;
			unsigned int numIndices;

		public:
			/**
			 * Constructor.
			 *
			 * @param myVBO 'Vertex'/position Buffer Object of this vertex buffer collection.
			 * @param myIBO Indice Buffer Object of this vertex buffer collection.
			 * @param myNumIndices Number of indices associated with this vertex buffer collection.
			 */
			VertexBuffer(GLuint myVBO, GLuint myIBO, GLuint myNBO, unsigned int myNumIndices);

			/**
			 * Request the 'Vertex'/position Buffer Object.
			 *
			 * @return 'Vertex'/position Buffer Object identifier.
			 */
			inline GLuint getVBO() const;

			/**
			 * Request the Indice Buffer Object.
			 *
			 * @return Indice Buffer Object identifier.
			 */
			inline GLuint getIBO() const;

			/**
			 * Request the number of indices.
			 *
			 * @return Number of indices.
			 */
			inline unsigned int getNumIndices() const;
	};

	GLuint VertexBuffer::getVBO() const {
		return vBO;
	}

	GLuint VertexBuffer::getIBO() const {
		return iBO;
	}

	unsigned int VertexBuffer::getNumIndices() const {
		return numIndices;
	}
}

#endif // VERTEXBUFFER_HPP
