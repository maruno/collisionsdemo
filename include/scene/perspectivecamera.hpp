#ifndef PERSPECTIVECAMERA_HPP
#define PERSPECTIVECAMERA_HPP

#include <glm/glm.hpp>

namespace scene {
	class SceneGroup;
	
	/**
	 * Main scene camera.
	 *
	 * @author Michel Bouwmans
	 */
	class PerspectiveCamera {
		private:
			glm::mat4 view, projection, viewProjection;
			static const glm::vec3 up;
			
			SceneGroup* world;
		public:
			/**
			 * Constructor
			 * 
			 * @param myWorld The world that's observed by this camera.
			 */
			PerspectiveCamera(SceneGroup* myWorld);
			
			/**
			 * Call this function when the viewport has been rescaled.
			 *
			 * @param width New width of the viewport.
			 * @param height New height of the viewport.
			 */
			void rescale(int width, int height);
			
			/**
			 * Change the position of the camera in the world.
			 * 
			 * @param position New camera position.
			 * @param direction New direction in which camera is observing.
			 */
			void changeCameraPosition(glm::vec3 position, glm::vec3 direction);
			
			/**
			 * Render an image using this camera.
			 */
			void render();
	};
}
#endif // PERSPECTIVECAMERA_HPP
