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
: childGroups{nullptr}, constraints(new AABB(myConstraints)), sceneDepth{0} {
	addOctreeLayers(octreeLevels, sceneDepth);

	rootNode = this;
}

void SceneGroup::addOctreeLayers(unsigned int levels, unsigned int myDepth) {
	assert(childGroups == nullptr);

	sceneDepth = ++myDepth;

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
				      child.addOctreeLayers(levels-1, sceneDepth);
			      });
	}
}

void SceneGroup::visitScene(std::function<void(std::shared_ptr<SceneItem>)> visitation) {
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

void SceneGroup::bubbleItem(std::shared_ptr<SceneItem> item) {
	std::lock_guard<std::recursive_mutex> guard(sceneMutex);

	if(childGroups != nullptr) {
		std::function<bool(SceneGroup&)> itemInGroup = [&item](SceneGroup& child) {
			const collisiondetection::BoundingVolume& otherBounds = item->getBounds();
			otherBounds.attachToItem(item.get());

			return otherBounds.intersects(*(child.constraints.get()));
		};

		unsigned short viableGroupIdx = 0;

		//std::array<SceneGroup, 8>::iterator viableGroup = std::find_if(childGroups->begin(), childGroups->end(), itemInGroup);
		//BUG workaround TROLOLOL Iterator is een pointer: typedef value_type* iterator;
		for(; viableGroupIdx < 8; ++viableGroupIdx) {
			if(itemInGroup(childGroups->at(viableGroupIdx))) {
				break;
			}
		}

		if(viableGroupIdx == 8) {
			if(this == rootNode) {
				addItem(item);
				return;
			} else {
				assert(viableGroupIdx != 8);
			}
		}


		if(viableGroupIdx < 7) {
		 	//auto otherGroup = std::find_if(viableGroup+1, childGroups->end(), itemInGroup);
			//BUG workaround TROLOLOL Iterator is een pointer: typedef value_type* iterator;
			unsigned short otherGroupIdx = viableGroupIdx+1;
			for(; otherGroupIdx < 8; ++otherGroupIdx) {
				if(itemInGroup((*childGroups)[otherGroupIdx])) {
					break;
				}
			}

			if(otherGroupIdx == 8) {
				//The item is on the edge of two groups, add to current group
				addItem(item);
				return;
			}
		}

		(*childGroups)[viableGroupIdx].bubbleItem(item);
	}

	addItem(item);
}

void SceneGroup::addItem(std::shared_ptr<SceneItem> item) {
	if (childGroups == nullptr && childItems.size() == config::globals::maxSceneGroupSize && sceneDepth != config::globals::maxSceneGraphDepth) {
		addOctreeLayers(1, sceneDepth);

		auto it = childItems.begin();
		while(it != childItems.end()) {
			std::lock_guard<std::recursive_mutex> guard(rootNode->sceneMutex);
			bubbleItem(*it);
			it = childItems.erase(it);
		}

		//Finally push in item
		std::lock_guard<std::recursive_mutex> lock(rootNode->sceneMutex);
		bubbleItem(item);
	} else {
		childItems.push_back(item);
	}
}

SceneGroup::~SceneGroup() {
	if(childGroups != nullptr) {
		delete childGroups;
	}
}