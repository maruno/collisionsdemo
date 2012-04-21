#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "glload/gl_3_2.h"
#include <glm/glm.hpp>

#include "scene/world.hpp"

namespace sceneitems {
	enum Rotation {LEFT, RIGHT, STATIONARY};

	class Terrain {
		private:
			GLuint vaos[1], vbos[1], ubos[1], shaderProgram;
			glm::mat4 modelMatrix, parentMatrix;

			Rotation rotation;

			unsigned int vertexCount;
		public:
			Terrain(const glm::mat4 myParentMatrix);

			void rotateLeft();
			void rotateRight();
			void stopRotation();

			void updateMatrix();

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
