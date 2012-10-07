#ifndef GLOBALS_HPP
#define GLOBALS_HPP

namespace config {
	
	namespace globals {
		constexpr unsigned short updateRate = 40;
		constexpr unsigned short frameRate = 35;
		constexpr unsigned int initialWidth = 1024;
		constexpr unsigned int initialHeight = 768;
		constexpr unsigned short multiSamples = 8;
		constexpr unsigned short cameraMovementSpeed = 2;
		constexpr float cameraRotationalSpeed = 0.5f;
		constexpr float ambianceLight = 0.05f;
	}
}

#endif // GLOBALS_HPP
