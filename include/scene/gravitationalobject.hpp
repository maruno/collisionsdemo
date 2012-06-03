#ifndef GRAVITATIONALOBJECT_HPP
#define GRAVITATIONALOBJECT_HPP

#include <glm/glm.hpp>

namespace scene {
	class GravitationalObject {
		private:
			unsigned int mass;
			glm::vec3 gravitationalForce;

		public:
			GravitationalObject(unsigned int myMass);

			inline unsigned int getMass() const;

			inline void addForce(glm::vec3 additionalForce);
			inline void resetForce();

			glm::vec3 currentMotion() const;
	};

	void GravitationalObject::addForce(glm::vec3 additionalForce) {
		gravitationalForce += additionalForce;
	}

	void GravitationalObject::resetForce() {
		gravitationalForce = glm::vec3();
	}

	unsigned int GravitationalObject::getMass() const {
		return mass;
	}
}

#endif // GRAVITATIONALOBJECT_HPP
