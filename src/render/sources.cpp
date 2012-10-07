#include "render/sources.hpp"

#include "scene/perspectivecamera.hpp"

using namespace render;

Sources* Sources::instance;

Sources::Sources(): needsUpload(true), numLightSources(0) {
	glGenBuffers(1, &uBO);
}

void Sources::cameraMoved() {
	sourcesUni.viewSource = scene::PerspectiveCamera::getInstance().getPosition();
	
	needsUpload = true;
}

unsigned int Sources::addLightSource(const glm::vec3& lightLocation, float lightIntensity) {
	if(numLightSources == 10) {
		throw MaxLightsReached();
	}
	
	numLightSources++;
	
	sourcesUni.lightSource[numLightSources] = glm::vec4(lightLocation, lightIntensity);
	
	needsUpload = true;
	
	return numLightSources;
}

void Sources::moveLightSource(unsigned int lightSourceId, const glm::vec3& lightLocation) {
	sourcesUni.lightSource[lightSourceId].x = lightLocation.x;
	sourcesUni.lightSource[lightSourceId].y = lightLocation.y;
	sourcesUni.lightSource[lightSourceId].z = lightLocation.z;
	
	needsUpload = true;
}

void Sources::upload() {
	if(needsUpload) {
		glBindBuffer(GL_UNIFORM_BUFFER, uBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(SourcesUni), &sourcesUni, GL_DYNAMIC_DRAW);
		
		needsUpload = false;
	}
}