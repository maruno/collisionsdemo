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
	TunnelSegment(const TunnelSegment& other);

	inline const glm::vec3& getDifficultyVector();

	virtual void update(){};

	//TODO Inline is a workaround for a clang 3.1-bug
	virtual inline ~TunnelSegment() = default;
};

const glm::vec3& TunnelSegment::getDifficultyVector() {
	return difficultyVector;
}