#ifndef GENERICPLANET_HPP
#define GENERICPLANET_HPP

#include "glload/gl_3_2.h"
#include <glm/glm.hpp>

#include "sceneitems/planet.hpp"

namespace sceneitems {
	class GenericPlanet : public sceneitems::Planet {
		private:
			GLuint colourUBO;
		public:
			/**
			 * Constructor.
			 *
			 * @param initialLocation The initial location of this planet.
			 * @param mySize Size in 10km-radius increments.
			 */
			GenericPlanet(glm::vec3 initialLocation, unsigned int mySize);

			virtual void render(glm::mat4& parentMatrix) const override;
	};
}

#endif // GENERICPLANET_HPP
