#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "glload/gl_3_2.h"
#include <glm/glm.hpp>

#include "scene/world.hpp"

namespace sceneitems {
	class Terrain {
		private:
			GLuint vaos[1], vbos[1], ubos[1], shaderProgram;
			glm::mat4 modelMatrix, parentMatrix;

			unsigned int vertexCount;
		public:
			Terrain(const glm::mat4 myParentMatrix);

			void updateMatrix();

			void render() const;

			//TODO destructor
	};

}
#endif // TERRAIN_HPP
