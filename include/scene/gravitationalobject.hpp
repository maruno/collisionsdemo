#ifndef GRAVITATIONALOBJECT_HPP
#define GRAVITATIONALOBJECT_HPP

#include <glm/glm.hpp>

#include "scene/SceneItem.hpp"

namespace scene {
	class GravitationalObject : public scene::SceneItem {
		private:
			unsigned int mass;
			glm::vec3 gravitationalForce;

		public:
			GravitationalObject(SceneItem* parent, unsigned int myMass);

			inline unsigned int getMass() const;

			inline void addForce(glm::vec3 additionalForce);
			inline void resetForce();

			glm::vec3 currentMotion() const;
			
			virtual void update() override = 0;
			virtual void draw() override = 0;
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
