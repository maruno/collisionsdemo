#include "tunnelsegment.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

TunnelSegment::TunnelSegment(glm::vec3& myLocation, glm::vec3& difficulty) : render::ColouredPhongSceneItem(myLocation, "tunnel-segment", render::ColourInformationUniform{glm::vec3{140.0f, 140.0f, 140.0f}, 0.3f}),
difficultyVector(difficulty) {
	//Offset location based on difficultyVector
	location.x += difficultyVector.x;
	location.y += difficultyVector.y;

	//Generate obstacles based on difficultyVector (z)
	for(int i = 0; i < difficultyVector.z; ++i) {
		glm::mat4 obstacleMatrix;
		obstacleMatrix = glm::rotate(obstacleMatrix, i*35.0f, glm::vec3{0.0f, 1.0f, 0.0f});

		obstacles.push_back(std::make_pair(glm::vec3{0.0f, -1.0f, 0.0f}, obstacleMatrix));
	}
}

TunnelSegment::TunnelSegment(const TunnelSegment& other) : render::ColouredPhongSceneItem(other.location, "tunnel-segment", render::ColourInformationUniform{glm::vec3{140.0f, 140.0f, 140.0f}, 0.3f}),	obstacles(other.obstacles), difficultyVector(other.difficultyVector){
}