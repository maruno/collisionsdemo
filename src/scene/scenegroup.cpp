#include "scene/scenegroup.hpp"

#include <algorithm>

using namespace scene;

SceneGroup::SceneGroup(unsigned int octreeLevels) {
	addOctreeLayers(octreeLevels);
}

void SceneGroup::addOctreeLayers(unsigned int levels) {
	childGroups.reset(new std::array<SceneGroup,8>);

	std::for_each(childGroups->begin(), childGroups->end(),
	[&levels](SceneGroup& child) {
		child.addOctreeLayers(--levels);
	});
}

void SceneGroup::updateScene() {
	std::for_each(childItems.begin(), childItems.end(), [](std::unique_ptr<SceneItem>& child) {
		child->update();
	});

	if(childGroups != nullptr) {
		std::for_each(childGroups->begin(), childGroups->end(),
		[](SceneGroup& child) {
			child.updateScene();
		});
	}
}

void SceneGroup::drawScene() {
	std::for_each(childItems.begin(), childItems.end(), [](std::unique_ptr<SceneItem>& child) {
		child->draw();
	});

	if(childGroups != nullptr) {
		std::for_each(childGroups->begin(), childGroups->end(),
		[](SceneGroup& child) {
			child.drawScene();
		});
	}
}
