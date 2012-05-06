#ifndef RENDER_VERTEXBUFFERFACTORY_HPP
#define RENDER_VERTEXBUFFERFACTORY_HPP

#include <tuple>
#include <string>
#include <map>

#include "glload/gl_3_2.h"

namespace render {
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
#endif // RENDER_VERTEXBUFFERFACTORY_HPP
