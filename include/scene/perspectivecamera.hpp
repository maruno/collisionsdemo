#ifndef PERSPECTIVECAMERA_HPP
#define PERSPECTIVECAMERA_HPP

#include <glm/glm.hpp>

namespace scene {
	class SceneGroup;
	
	/**
	 * Scene world definition class
	 *
	 * @author Michel Bouwmans
	 */
	class PerspectiveCamera {
		private:
			glm::mat4 view, projection, viewProjection;
			static const glm::vec3 up;
			
			SceneGroup* world;
		public:
			PerspectiveCamera(SceneGroup* myWorld);
			
			/**
			 * Call this function when the viewport has been rescaled.
			 *
			 * @param width New width of the viewport.
			 * @param height New height of the viewport.
			 */
			void rescale(int width, int height);
			void changeCameraPosition(glm::vec3 position, glm::vec3 direction);
			
			void render();
	};
}
#endif // PERSPECTIVECAMERA_HPP
