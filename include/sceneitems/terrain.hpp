#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "glload/gl_3_2.h"
#include <glm/glm.hpp>

#include "scene/world.hpp"

namespace sceneitems {
	enum Rotation {LEFT, RIGHT, STATIONARY};

	/**
	 * Sceneitem class for a heightmap-generated terrain.
	 *
	 * @author Michel Bouwmans
	 */
	class Terrain {
		private:
			GLuint vaos[1], vbos[1], ubos[2], shaderProgram;
			glm::mat4 modelMatrix, parentMatrix;

			Rotation rotation;

			unsigned int vertexCount;
		public:
			/**
			 * Constructor.
			 *
			 * @param myParentMatrix HACK Matrix of my parent.
			 * @param size Size of the heightmap that should be generated.
			 * @param variance Variance of the Heightmap to be generated.
			 * @see HeightmapGenerator
			 */
			Terrain(const glm::mat4 myParentMatrix, unsigned int size, unsigned int variance);

			/**
			 * Start rotating left on each update.
			 */
			void rotateLeft();

			/**
			 * Start rotating right on each update.
			 */
			void rotateRight();

			/**
			 * Stop rotating.
			 */
			void stopRotation();

			/**
			 * Update all variables and send a new matrix to the UBO.
			 *
			 * HACK
			 */
			void updateMatrix();

			/**
			 * Render to the framebuffer
			 */
			void render() const;

			//TODO destructor
	};

	inline void Terrain::rotateLeft() {
		rotation = LEFT;
	}

	inline void Terrain::rotateRight() {
		rotation = RIGHT;
	}

	inline void Terrain::stopRotation() {
		rotation = STATIONARY;
	}
}
#endif // TERRAIN_HPP
