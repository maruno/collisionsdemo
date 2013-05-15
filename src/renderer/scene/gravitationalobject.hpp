#ifndef GRAVITATIONALOBJECT_HPP
#define GRAVITATIONALOBJECT_HPP

#include <mutex>

#include <glm/glm.hpp>

#include "renderer/render/colouredphongsceneitem.hpp"

namespace scene {
	/**
	 * This class represents a @ref SceneItem that obeys the laws of physics from Sir Isaac Newton.
	 *
	 * @author Michel Bouwmans
	 */
	class GravitationalObject : public render::ColouredPhongSceneItem {
	 private:
		unsigned int mass;
		glm::vec3 gravitationalForce;
		glm::vec3 currentMotion;

	 public:
		/**
		 * Constructor.
		 *
		 * @param myMass Mass of this item.
		 */
		GravitationalObject(glm::vec3 initialLocation, unsigned int myMass, std::string objectName, render::ColourInformationUniform colour);

		/**
		 * Request the mass.
		 *
		 * @return Mass of this item.
		 */
		inline unsigned int getMass() const;

		/**
		 * Add a new force to this item.
		 *
		 * @param additionalForce The additional force that works on this item.
		 */
		inline void addForce(glm::vec3 additionalForce);

		/**
		 * Reset all forces acting upon this item.
		 */
		inline void resetForce();

		/**
		 * Request the current motion.
		 *
		 * @return The current motion of this item.
		 */
		inline glm::vec3 getCurrentMotion() const;

		virtual void update() override;
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

	glm::vec3 GravitationalObject::getCurrentMotion() const {
		return currentMotion;
	}
}

#endif // GRAVITATIONALOBJECT_HPP
