#ifndef SOURCES_HPP
#define SOURCES_HPP

#include <stdexcept>

#include "glload/gl_3_2.h"
#include <glm/glm.hpp>

namespace render {
	struct SourcesUni {
		glm::vec3 viewSource;
		int padding;
		glm::vec4 lightSource[10]; //a = Intensity of incident light
	};

	class MaxLightsReached : public std::runtime_error {
		public:
			MaxLightsReached() : runtime_error("The maximum number of lights (10) has been reached.") {};
	};
	
	class Sources final {
		private:
			static Sources* instance;
			
			SourcesUni sourcesUni;
			unsigned int numLightSources;
			
			GLuint uBO;
			
			bool needsUpload;
			
			Sources();
		public:
			Sources(const Sources&) = delete;
			Sources& operator=(const Sources&) = delete;
			
			static inline Sources& getInstance();
			
			void cameraMoved();
			
			unsigned int addLightSource(const glm::vec3& lightLocation, float lightIntensity);
			void moveLightSource(unsigned int lightSourceId, const glm::vec3& lightLocation);
			
			void upload();
			
			inline GLuint getUBO();
	};
	
	Sources& Sources::getInstance() {
		if(instance == nullptr) {
			instance = new Sources;
		}
		
		return *instance;
	}
	
	GLuint Sources::getUBO() {
		return uBO;
	}
}

#endif // SOURCES_HPP
