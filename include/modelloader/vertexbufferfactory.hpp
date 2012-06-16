#ifndef MODELLOADER_VERTEXBUFFERFACTORY_HPP
#define MODELLOADER_VERTEXBUFFERFACTORY_HPP

#include <tuple>
#include <string>
#include <map>

#include "glload/gl_3_2.h"

namespace modelloader {
	/**
	 * Factory class for vertex buffers.
	 * 
	 * This class returns vertex buffers, associated with a name, from a flyweight pool if available.
	 * When the requested name is not present in the pool it tries to load it using @ref ObjParser.
	 * 
	 * @author Michel Bouwmans
	 */
	class VertexBufferFactory {
		private:
			std::map<const std::string, const std::tuple<const GLuint, const GLuint>> vbopool;
			
			static VertexBufferFactory instance;
			VertexBufferFactory() {};
		public:
			/**
			 * Access a vertex buffer by name.
			 * This method to parse an obj-file when not in the pool and fails on error.
			 * 
			 * @param objName Name of the obj-file without extension.
			 * @return The requested vertex buffer collection.
			 */
			const std::tuple<const GLuint, const GLuint> operator[] (std::string objName);
			
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
