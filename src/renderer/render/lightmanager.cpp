#include "lightmanager.hpp"

#include "scene/perspectivecamera.hpp"

using namespace render;

std::unique_ptr<LightManager> LightManager::instance;

LightManager::LightManager(): needsUpload(true), numLightSources(0) {
	glGenBuffers(1, &uBO);
}

/*void LightManager::cameraMoved() {
	sourcesUni.viewSource = scene::PerspectiveCamera::getInstance().getPosition();
	
	needsUpload = true;
}*/

unsigned int LightManager::addLightSource(const glm::vec3& lightLocation, float lightIntensity) {
	if(numLightSources == 10) {
		throw MaxLightsReached();
	}
	
	numLightSources++;
	
	lightSources[numLightSources-1] = glm::vec4(lightLocation, lightIntensity);
	
	needsUpload = true;
	
	return numLightSources;
}

void LightManager::moveLightSource(unsigned int lightSourceId, const glm::vec3& lightLocation) {
	glm::vec4& lightSource = lightSources[lightSourceId];
	
	lightSource.x = lightLocation.x;
	lightSource.y = lightLocation.y;
	lightSource.z = lightLocation.z;
	
	needsUpload = true;
}

void LightManager::upload() {
	if(needsUpload) {
		glBindBuffer(GL_UNIFORM_BUFFER, uBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 10, lightSources.data(), GL_DYNAMIC_DRAW);
		
		needsUpload = false;
	}
}