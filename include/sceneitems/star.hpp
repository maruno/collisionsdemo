#ifndef SCENEITEMS_STAR_HPP
#define SCENEITEMS_STAR_HPP

#include "sceneitems/planet.hpp"

#include <glm/glm.hpp>

namespace sceneitems {
	class Star : public sceneitems::Planet {
		private:
			GLuint colourUBO;
			
			unsigned int lightSourceId;
		public:
			/**
			 * Constructor.
			 *
			 * @param initialLocation The initial location of this planet.
			 * @param mySize Size in 10km-radius increments.
			 */
			Star(glm::vec3 initialLocation, unsigned int mySize);

			virtual void update() override;
			virtual void render(glm::mat4& parentMatrix) const override;
	};
}

#endif // SCENEITEMS_STAR_HPP
