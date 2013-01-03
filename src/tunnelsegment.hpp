#pragma once

#include <vector>
#include <utility>

#include <glm/glm.hpp>

#include "renderer/render/colouredphongsceneitem.hpp"

class TunnelSegment : public render::ColouredPhongSceneItem {
 private:
	const glm::vec3 difficultyVector;
	std::vector<std::pair<glm::vec3, glm::mat4>> obstacles;

 public:
	TunnelSegment(glm::vec3& location, glm::vec3& difficulty);

	inline const glm::vec3& getDifficultyVector();

	virtual void update(){};
};

const glm::vec3& TunnelSegment::getDifficultyVector() {
	return difficultyVector;
}