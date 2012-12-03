#ifndef MODELLOADER_VERTEXBUFFERFACTORY_HPP
#define MODELLOADER_VERTEXBUFFERFACTORY_HPP

#include <string>
#include <map>
#include <vector>

#include "glload/gl_3_2.h"
#include "glm/glm.hpp"

#include "modelloader/vertexbuffer.hpp"

namespace modelloader {

struct vec3Comparator {
	bool operator() (const glm::vec3& lhs, const glm::vec3& rhs) const {
		bool less = false;
		if(lhs.x < rhs.x)
			less = true;
		else if(lhs.y < rhs.y)
			less = true;
		else if(lhs.z < rhs.z)
			less = true;
		return less;
	}
};

	/**
	 * Factory class for vertex buffers.
	 *
	 * This class returns vertex buffer collections, associated with a name, from a flyweight pool if available.
	 * When the requested name is not present in the pool it tries to load it using @ref ObjParser.
	 *
	 * @author Michel Bouwmans
	 */
	class VertexBufferFactory {
		private:
			std::map<const std::string, VertexBuffer> vbopool;

			static VertexBufferFactory instance;
			VertexBufferFactory() = default;
			
			std::vector<glm::vec3> calculateVertexNormals(std::vector<unsigned int>& indicesData, std::vector<glm::vec3>& verticesData);
		public:
			/**
			 * Access a vertex buffer by name.
			 * This method to parse an obj-file when not in the pool and fails on error.
			 *
			 * @param objName Name of the obj-file without extension.
			 * @return The requested vertex buffer collection.
			 */
			const VertexBuffer& operator[](std::string objName);

			/**
			 * Get the instance of this VertexBufferFactory.
			 */
			static inline VertexBufferFactory& getInstance();
	};

	inline VertexBufferFactory& VertexBufferFactory::getInstance() {
		return instance;
	}
}
#endif // MODELLOADER_VERTEXBUFFERFACTORY_HPP
