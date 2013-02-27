#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <map>
#include <string>
#include "modelloader/vertexbufferfactory.hpp"

namespace config {
	namespace globals {
		constexpr unsigned short updateRate = 40;
		constexpr unsigned short frameRate = 35;
		constexpr unsigned int initialWidth = 1024;
		constexpr unsigned int initialHeight = 768;
		constexpr unsigned short multiSamples = 8;
		constexpr unsigned short cameraMovementSpeed = 2;
		constexpr float cameraRotationalSpeed = 0.5f;
		constexpr float ambianceLight = 0.2f;
		constexpr unsigned int tunnelLength = 100;
		constexpr unsigned int maxSceneGroupSize = 20;
	}

	const std::map<std::string, modelloader::DataPreservation> preservationRules{{std::string{"tunnel-segment"}, modelloader::DataPreservation::vertexAndNormal}};
}

#endif // GLOBALS_HPP
