#include "tunnelgenerator.hpp"

#include <boost/random/uniform_int_distribution.hpp>

TunnelGenerator::TunnelGenerator() {
	++(*this);
}

TunnelGenerator& TunnelGenerator::operator++() {
	//Segment offset in tunnel
	glm::vec3 segmentLocation{0.0f, 0.0f, 0.0f};

	if(current != nullptr) {
		segmentLocation = current->getLocation();
		segmentLocation += glm::vec3{0.0f, 0.0f, -0.1f};
	}

	//Calculate difficulty of tunnel segment
	//TODO Add probabilities and use information from current
	glm::vec3 difficultyVector;

	boost::random::uniform_int_distribution<> positionDist(1,6);
	difficultyVector.x = (positionDist(gen)-3)/10.0f;
	difficultyVector.y = (positionDist(gen)-3)/10.0f;

	boost::random::uniform_int_distribution<> obstaclesDist(1,4);
	difficultyVector.z = obstaclesDist(gen)/10.0f;

	//Invalidates old iterator and creates new current
	current.reset(new TunnelSegment{segmentLocation, difficultyVector});

	return *this;
}