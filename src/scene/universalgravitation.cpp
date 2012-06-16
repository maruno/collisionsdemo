#include "scene/universalgravitation.hpp"

#include <glm/glm.hpp>
#include <algorithm>

using namespace scene;

void UniversalGravitation::update() {
	std::for_each(gravityObjects.begin(), gravityObjects.end(), [](GravitationalObject* gravObject) {
		gravObject->resetForce();
	});

	//Calculate gravity for each unique combination
	for(int n = 0; n < gravityObjects.size(); ++n) {
		for(int m = n+1; m < gravityObjects.size(); ++m) {
			GravitationalObject* gravObject1 = gravityObjects[n];
			GravitationalObject* gravObject2 = gravityObjects[m];

			float r = glm::distance(gravObject1->getLocation(), gravObject2->getLocation());

			//Newton's law of universal gravitation
			float F = (gravObject1->getMass() * gravObject2->getMass() * G) / r*r;

			//Create directional forces
			glm::vec3 gravForce1 = glm::normalize(gravObject2->getLocation() - gravObject1->getLocation());
			glm::vec3 gravForce2 = glm::normalize(gravObject1->getLocation() - gravObject2->getLocation());
			gravForce1 *= F;
			gravForce2 *= F;

			gravObject1->addForce(gravForce1);
			gravObject2->addForce(gravForce2);
		}
	}
}
