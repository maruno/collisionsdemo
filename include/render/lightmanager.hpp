#ifndef LIGHTMANAGER_HPP
#define LIGHTMANAGER_HPP

#include <stdexcept>
#include <array>

#include "glload/gl_3_2.h"
#include <glm/glm.hpp>

namespace render {
	class MaxLightsReached : public std::runtime_error {
		public:
			MaxLightsReached() : runtime_error("The maximum number of lights (10) has been reached.") {};
	};
	
	class LightManager final {
		private:
			static LightManager* instance;
			
			std::array<glm::vec4, 10> lightSources; //a = Intensity of incident light
			unsigned int numLightSources;
			
			GLuint uBO;
			bool needsUpload;
			
			LightManager();
		public:
			LightManager(const LightManager&) = delete;
			LightManager& operator=(const LightManager&) = delete;
			
			static inline LightManager& getInstance();
			
			//void cameraMoved();
			
			unsigned int addLightSource(const glm::vec3& lightLocation, float lightIntensity);
			void moveLightSource(unsigned int lightSourceId, const glm::vec3& lightLocation);
			
			void upload();
			
			inline GLuint getUBO();
	};
	
	LightManager& LightManager::getInstance() {
		if(instance == nullptr) {
			instance = new LightManager;
		}
		
		return *instance;
	}
	
	GLuint LightManager::getUBO() {
		return uBO;
	}
}

#endif // LIGHTMANAGER_HPP
