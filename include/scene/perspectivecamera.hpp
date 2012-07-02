#ifndef PERSPECTIVECAMERA_HPP
#define PERSPECTIVECAMERA_HPP

#include <glm/glm.hpp>

namespace scene {
	class SceneGroup;

	/**
	 *
	 * Main scene camera.
	 *
	 * @author Michel Bouwmans
	 */
	class PerspectiveCamera {
		public:
			enum KeyPress {
				NO_KEY_PRESSED, UP_KEY_PRESSED, DOWN_KEY_PRESSED, LEFT_KEY_PRESSED,
				RIGHT_KEY_PRESSED, W_KEY_PRESSED, A_KEY_PRESSED, S_KEY_PRESSED,
				D_KEY_PRESSED, PAGEUP_KEY_PRESSED, PAGEDOWN_KEY_PRESSED
			};

		private:
			glm::mat4 view, projection, viewProjection;
			static const glm::vec3 up;

			glm::vec3 position;
			glm::vec3 direction;

			SceneGroup* world;

			static PerspectiveCamera::KeyPress keyPressed;

			void updatePosition(float dX, float dY, float dZ);
			void updateDirection(float angle, float x, float y, float z);
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

			/**
			 * Update the position and direction of the camera.
			 */
			void update();

			void setKeyPressed(KeyPress key) {
				keyPressed = key;
			};
	};
}
#endif // PERSPECTIVECAMERA_HPP
