#ifndef MATRIXUNIFORM_HPP
#define MATRIXUNIFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "scene/perspectivecamera.hpp"

namespace render {
	struct MatrixUniform {
		glm::mat4 mVPMatrix;
		glm::mat4 mVMatrix;
		glm::mat4 normalMatrix;

		MatrixUniform(const glm::mat4& myMVMatrix) 
		: mVMatrix(myMVMatrix), mVPMatrix(scene::PerspectiveCamera::getInstance().getProjectionMatrix() * myMVMatrix) {
			normalMatrix = glm::inverseTranspose(myMVMatrix);
		};
	};
}

#endif // MATRIXUNIFORM_HPP
