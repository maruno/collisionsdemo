#ifndef PLANET_HPP
#define PLANET_HPP

#include "glload/gl_3_2.h"
#include <glm/glm.hpp>

#include "scene/gravitationalobject.hpp"

#include "modelloader/vertexbuffer.hpp"

namespace sceneitems {
	class Planet : public scene::GravitationalObject {
		private:
			const modelloader::VertexBuffer& vBuffers;
			unsigned int size;
			static GLuint vao;
			GLuint colourUBO;
		public:
			/**
			 * Constructor.
			 * 
			 * @param initialLocation The initial location of this planet.
			 * @param mySize Size in 10km-radius increments.
			 * @param density Density multiplier.
			 */
			Planet(glm::vec3 initialLocation, unsigned int mySize, unsigned int density);

			virtual void update() override;
			virtual void render(glm::mat4& parentMatrix) const override;
	};
}

#endif // PLANET_HPP
