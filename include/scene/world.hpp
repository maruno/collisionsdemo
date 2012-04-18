#ifndef WORLD_HPP
#define WORLD_HPP

#include <glm/glm.hpp>

namespace scene {
	class World {
		private:
			static const glm::mat4 view;
			static glm::mat4 viewProjection;
		public:
			static void rescale(int width, int height);
			static const glm::mat4& getMatrix();
	};

	inline const glm::mat4& World::getMatrix() {
		return viewProjection;
	}
}
#endif // WORLD_HPP
