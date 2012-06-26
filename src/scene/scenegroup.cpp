#include "scene/scenegroup.hpp"

#include <algorithm>

using namespace scene;

SceneGroup::SceneGroup(unsigned int octreeLevels) {
	addOctreeLayers(octreeLevels);
}

void SceneGroup::addOctreeLayers(unsigned int levels) {
	if(levels != 0) {
		childGroups.reset(new std::array<SceneGroup,8>);

		std::for_each(childGroups->begin(), childGroups->end(),
		[&levels](SceneGroup& child) {
			child.addOctreeLayers(--levels);
		});
	}
}

void SceneGroup::visitScene(std::function<void(std::unique_ptr<SceneItem>&)> visitation) {
	std::for_each(childItems.begin(), childItems.end(), visitation);
	
	if(childGroups != nullptr) {
		std::for_each(childGroups->begin(), childGroups->end(),
		[&](SceneGroup& child) {
			child.visitScene(visitation);
		});
	}
}
