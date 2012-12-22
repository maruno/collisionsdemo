#ifndef COLOURINFORMATIONUNIFORM_HPP
#define COLOURINFORMATIONUNIFORM_HPP

#include <glm/glm.hpp>

#include "config/globals.hpp"

namespace render {
	struct ColourInformationUniform {
		glm::vec3 ambiance;
		int padding;
		glm::vec3 kColour;
		int padding2;
		glm::vec3 mShininess;

		ColourInformationUniform(const glm::vec3& colour, float shininess)
			: kColour(1.0f, 1.0f, 1.0f), mShininess(shininess), ambiance(config::globals::ambianceLight) {
			kColour.r -= colour.r/255.0f;
			kColour.g -= colour.g/255.0f;
			kColour.b -= colour.b/255.0f;
		};
	};
}

#endif // COLOURINFORMATIONUNIFORM_HPP
