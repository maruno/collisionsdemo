#ifndef PERSPECTIVECAMERA_HPP
#define PERSPECTIVECAMERA_HPP

#include <memory>

#include "glload/gl_3_2.h"
#include <glm/glm.hpp>

namespace scene {
	class SceneGroup;

	/**
	 *
	 * Main scene camera.
	 *
	 * @author Michel Bouwmans
	 */
	class PerspectiveCamera final {
	 private:
		static std::unique_ptr<PerspectiveCamera> instance;

		glm::mat4 view, projection;

		glm::vec3 up;
		glm::vec3 position;
		glm::vec3 direction;

		unsigned int lightID;

		GLuint uBO;
		bool needsUpload;

		PerspectiveCamera();

		void upload();

		void updatePosition(float dX, float dY, float dZ);
		void updateDirection(float angle, float x, float y, float z);
	 public:
		PerspectiveCamera(const PerspectiveCamera&) = delete;
		PerspectiveCamera& operator=(const PerspectiveCamera&) = delete;

		static inline PerspectiveCamera& getInstance();

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
		void changeCameraPosition(glm::vec3 myPosition, glm::vec3 myDirection);

		/**
		 * Render an image using this camera.
		 */
		void render(scene::SceneGroup* world);

		inline GLuint getUBO();

		inline const glm::vec3& getPosition();

		inline const glm::mat4& getProjectionMatrix();
	};

	PerspectiveCamera& PerspectiveCamera::getInstance() {
		if(instance == nullptr) {
			instance.reset(new PerspectiveCamera);
		}

		return *instance;
	}

	const glm::vec3& PerspectiveCamera::getPosition() {
		return position;
	}

	const glm::mat4& PerspectiveCamera::getProjectionMatrix() {
		return projection;
	}

	GLuint PerspectiveCamera::getUBO() {
		return uBO;
	}
}
#endif // PERSPECTIVECAMERA_HPP
