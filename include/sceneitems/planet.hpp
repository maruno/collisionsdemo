#ifndef PLANET_HPP
#define PLANET_HPP

#include "scene/gravitationalobject.hpp"

#include <glm/glm.hpp>

#include "modelloader/vertexbuffer.hpp"

namespace sceneitems {
	class Planet : public scene::GravitationalObject {
		private:
			unsigned int size;
		protected:
			const modelloader::VertexBuffer& vBuffers;
			static GLuint vao;
		public:
			Planet(glm::vec3 initialLocation, unsigned int mySize, unsigned int density);
			
			virtual void render(glm::mat4& parentMatrix) const override = 0;
			virtual void update() override;
	};
}

#endif // PLANET_HPP
