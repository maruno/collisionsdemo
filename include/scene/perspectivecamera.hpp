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

			glm::vec3 position;
			glm::vec3 direction;

			SceneGroup* world;

			static char keyPressed;

			void updatePosition(float dX, float dY, float dZ);
			void updateDirection(float angle, float x, float y, float z);
		public:
			static const char NO_KEY_PRESSED = 0;
			static const char UP_KEY_PRESSED = 1;
			static const char DOWN_KEY_PRESSED = 2;
			static const char LEFT_KEY_PRESSED = 3;
			static const char RIGHT_KEY_PRESSED = 4;
			static const char W_KEY_PRESSED = 5;
			static const char A_KEY_PRESSED = 6;
			static const char S_KEY_PRESSED = 7;
			static const char D_KEY_PRESSED = 8;
			static const char PAGEUP_KEY_PRESSED = 9;
			static const char PAGEDOWN_KEY_PRESSED = 10;
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

			void setKeyPressed(const char key) { keyPressed = key; };
	};
}
#endif // PERSPECTIVECAMERA_HPP
