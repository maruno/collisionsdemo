#ifndef WORLD_HPP
#define WORLD_HPP

#include <glm/glm.hpp>

namespace scene {
	/**
	 * Scene world definition class
	 *
	 * @author Michel Bouwmans
	 */
	class World {
		private:
			static const glm::mat4 view;
			static glm::mat4 viewProjection;
		public:
			/**
			 * Call this function when the viewport has been rescaled.
			 *
			 * @param width New width of the viewport.
			 * @param height New height of the viewport.
			 */
			static void rescale(int width, int height);

			/**
			 * Get the world matrix.
			 *
			 * @return World matrix
			 */
			static const glm::mat4& getMatrix();
	};

	inline const glm::mat4& World::getMatrix() {
		return viewProjection;
	}
}
#endif // WORLD_HPP
