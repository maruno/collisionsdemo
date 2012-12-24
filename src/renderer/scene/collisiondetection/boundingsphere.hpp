/*
 * BoundingSphere.hpp
 *
 *      Author: Marcel Veltman
 */

#ifndef BOUNDINGSPHERE_HPP_
#define BOUNDINGSPHERE_HPP_

#include <glm/glm.hpp>

#include "scene/collisiondetection/boundingvolume.hpp"

namespace scene {
	namespace collisiondetection {
		class BoundingSphere : public BoundingVolume {
		 public:
			BoundingSphere(float radius, glm::vec3 centreLocation);
			virtual ~BoundingSphere();
			inline glm::vec3 getLocation() const { return location; };
			inline float getRadius() const { return radius; };
		 private:
			float radius;
			glm::vec3 location;
		};
	}
}

#endif /* BOUNDINGSPHERE_HPP_ */
