#ifndef PLANET_HPP
#define PLANET_HPP

#include "scene/gravitationalobject.hpp"

#include <glm/glm.hpp>

#include "modelloader/vertexbuffer.hpp"

#include "scene/collisiondetection/boundingvolume.hpp"
#include "scene/collisiondetection/boundingsphere.hpp"

namespace sceneitems {
	class Planet : public scene::GravitationalObject {
		private:
			unsigned int size;
			scene::collisiondetection::BoundingSphere bSphere;

		public:
			Planet(glm::vec3 initialLocation, unsigned int mySize, float density);
			
			virtual void render(glm::mat4& parentMatrix) const override = 0;
			virtual void move() override;
	};
}

#endif // PLANET_HPP
