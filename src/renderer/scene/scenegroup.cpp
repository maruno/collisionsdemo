#include "scene/scenegroup.hpp"

#include <algorithm>
#include <tuple>
#include <functional>
#include <cassert>

#include "config/globals.hpp"

#include "sceneitem.hpp"

using namespace scene;

typedef collisiondetection::AxisAlignedBoundingBox AABB;
typedef std::tuple<glm::vec3, glm::vec3> Diagonal;

SceneGroup* SceneGroup::rootNode;
std::recursive_mutex SceneGroup::sceneMutex;

SceneGroup::SceneGroup() : childGroups{nullptr} {
	rootNode = this;
}

SceneGroup::SceneGroup(unsigned int octreeLevels, collisiondetection::AxisAlignedBoundingBox myConstraints)
	: constraints(new AABB(myConstraints)) {
	addOctreeLayers(octreeLevels);

	rootNode = this;
}

void SceneGroup::addOctreeLayers(unsigned int levels) {
	if(levels != 0) {
		childGroups = new std::array<SceneGroup,8>;

		//Calculate and set constraints for all childGroups
		float width = (constraints->maxX - constraints->minX) / 2.0f;
		float height = (constraints->maxY - constraints->minY) / 2.0f;
		float depth = (constraints->maxZ - constraints->minZ) / 2.0f;

		glm::vec3 childMinimum(constraints->minX, constraints->minY, constraints->maxZ);
		glm::vec3 childMaximum(constraints->minX + width, constraints->minY + height, constraints->maxZ - depth);

		(*childGroups)[0].constraints.reset(new AABB(Diagonal(childMinimum, childMaximum)));

		childMinimum.x += width;
		childMaximum.x += width;
		(*childGroups)[1].constraints.reset(new AABB(Diagonal(childMinimum, childMaximum)));

		childMinimum.z -= depth;
		childMaximum.z -= depth;
		(*childGroups)[2].constraints.reset(new AABB(Diagonal(childMinimum, childMaximum)));

		childMinimum.x -= width;
		childMaximum.x -= width;
		(*childGroups)[3].constraints.reset(new AABB(Diagonal(childMinimum, childMaximum)));

		childMinimum.y += height;
		childMaximum.y += height;
		(*childGroups)[4].constraints.reset(new AABB(Diagonal(childMinimum, childMaximum)));

		childMinimum.z += depth;
		childMaximum.z += depth;
		(*childGroups)[5].constraints.reset(new AABB(Diagonal(childMinimum, childMaximum)));

		childMinimum.x += width;
		childMaximum.x += width;
		(*childGroups)[6].constraints.reset(new AABB(Diagonal(childMinimum, childMaximum)));

		childMinimum.z -= depth;
		childMaximum.z -= depth;
		(*childGroups)[7].constraints.reset(new AABB(Diagonal(childMinimum, childMaximum)));

		//Recursive call
		std::for_each(childGroups->begin(), childGroups->end(),
		[=](SceneGroup& child) {
			child.addOctreeLayers(levels-1);
		});
	}
}

void SceneGroup::visitScene(std::function<void(std::unique_ptr<SceneItem>&)> visitation) {
	std::lock_guard<std::recursive_mutex> guard(sceneMutex);

	std::for_each(childItems.begin(), childItems.end(), visitation);

	if(childGroups != nullptr) {
		std::for_each(childGroups->begin(), childGroups->end(),
		[&](SceneGroup& child) {
			child.visitScene(visitation);
		});
	}
}

void SceneGroup::visitGroups(std::function<void(SceneGroup&)> visitation) {
	visitation(*this);

	if(childGroups != nullptr) {
		std::for_each(childGroups->begin(), childGroups->end(),
		[&](SceneGroup& child) {
			child.visitGroups(visitation);
		});
	}
}

void SceneGroup::bubbleItem(std::unique_ptr<SceneItem> item) {
	std::lock_guard<std::recursive_mutex> guard(sceneMutex);
	
	if(childGroups != nullptr) {
		std::function<bool(SceneGroup&)> itemInGroup = [&item](SceneGroup& child) {
			return child.constraints->intersects(item->getBounds());
		};

		auto viableGroup = std::find_if(childGroups->begin(), childGroups->end(), itemInGroup);

		assert(viableGroup != childGroups->end());

		if(viableGroup+1 != childGroups->end()) {
			auto otherGroup = std::find_if(viableGroup+1, childGroups->end(), itemInGroup);

			if(otherGroup != childGroups->end()) {
				//The item is on the edge of two groups, add to current group
				addItem(std::move(item));
				return;
			}
		}

		viableGroup->bubbleItem(std::move(item));
	}

	addItem(std::move(item));
}

void SceneGroup::addItem(std::unique_ptr<SceneItem> item) {
	if (childGroups == nullptr && childItems.size() == config::globals::maxSceneGroupSize) {
		addOctreeLayers(1);

		auto it = childItems.begin();
		while(it != childItems.end()) {
			std::lock_guard<std::recursive_mutex> guard(rootNode->sceneMutex);
			bubbleItem(std::move(*it));
			it = childItems.erase(it);
		}

		//Finally push in item
		std::lock_guard<std::recursive_mutex> lock(rootNode->sceneMutex);
		bubbleItem(std::move(item));
	} else {
		childItems.push_back(std::move(item));
	}
}

SceneGroup::~SceneGroup() {
	if(childGroups != nullptr) {
		delete childGroups;
	}
}