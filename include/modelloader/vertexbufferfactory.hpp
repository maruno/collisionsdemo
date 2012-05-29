#ifndef MODELLOADER_VERTEXBUFFERFACTORY_HPP
#define MODELLOADER_VERTEXBUFFERFACTORY_HPP

#include <tuple>
#include <string>
#include <map>

#include "glload/gl_3_2.h"

namespace modelloader {
	class VertexBufferFactory {
		private:
			std::map<const std::string, const std::tuple<const GLuint, const GLuint>> vbopool;
			
			static VertexBufferFactory instance;
			VertexBufferFactory() {};
		public:
			const std::tuple<const GLuint, const GLuint> operator[] (std::string objName);
			
			VertexBufferFactory& getInstance();
	};
	
	VertexBufferFactory& VertexBufferFactory::getInstance() {
 		return instance;
	}
}
#endif // MODELLOADER_VERTEXBUFFERFACTORY_HPP
