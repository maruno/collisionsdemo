#include "scene/scenegroup.hpp"

#include <algorithm>
#include <tuple>
#include <functional>
#include <cassert>
#include <typeinfo>

#include <dispatch/dispatch.h>

#include "config/globals.hpp"

#include "sceneitem.hpp"

#include "collisiondetection/collidable.hpp"

using namespace scene;

typedef collisiondetection::AxisAlignedBoundingBox AABB;
typedef std::tuple<glm::vec3, glm::vec3> Diagonal;

SceneGroup* SceneGroup::rootNode;
std::recursive_mutex SceneGroup::sceneMutex;

extern dispatch_queue_t gcd_queue;

SceneGroup::SceneGroup() : childGroups{nullptr}, parent{nullptr} {
	rootNode = this;
}

SceneGroup::SceneGroup(unsigned int octreeLevels, collisiondetection::AxisAlignedBoundingBox myConstraints)
: childGroups{nullptr}, constraints(new AABB(myConstraints)), sceneDepth{0}, parent{nullptr} {
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

				      child.parent = this;
			      });
	}
}

void SceneGroup::visitScene(std::function<void(std::shared_ptr<SceneItem>&)> visitation) {
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

void SceneGroup::visitParentGroups(std::function<void(SceneGroup &)> visitation) {
	std::vector<SceneGroup*> parents;
	SceneGroup* currentParent = parent;

	while(currentParent != nullptr) {
		parents.push_back(currentParent);
		currentParent = currentParent->parent;
	}

	for(SceneGroup* p : parents) {
		visitation(*p);
	}
}

void SceneGroup::bubbleItem(std::shared_ptr<SceneItem>& item) {
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
				//Item probably out of scene graph root boundaries or on the edge of two groups
				addItem(item);

				tryRelocationLater(item);

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

				tryRelocationLater(item);

				return;
			}
		}

		(*childGroups)[viableGroupIdx].bubbleItem(item);
	}

	addItem(item);
}

void SceneGroup::addItem(std::shared_ptr<SceneItem>& item) {
	if (childGroups == nullptr && childItems.size() == config::globals::maxSceneGroupSize && sceneDepth != config::globals::maxSceneGraphDepth) {
		std::lock_guard<std::recursive_mutex> lock(rootNode->sceneMutex);

		addOctreeLayers(1, sceneDepth);

		std::vector<std::shared_ptr<SceneItem>> bubbleCandidates {item};
		std::move(childItems.begin(), childItems.end(), std::back_inserter(bubbleCandidates));
		childItems.clear();

		//Finally push in item
		for(std::shared_ptr<SceneItem>& candidate : bubbleCandidates) {
			bubbleItem(candidate);
		}
	} else {
		childItems.push_back(item);
	}
}

void SceneGroup::tryRelocationLater(std::shared_ptr<SceneItem> item) {
	//For now only detect collidables, because only those move right now.
	if(typeid(*item) == typeid(collisiondetection::Collidable)) {
		auto when = dispatch_time(DISPATCH_TIME_NOW, 500000000);
		dispatch_after(when, gcd_queue, ^{
			auto it = std::find(childItems.begin(), childItems.end(), item);

			if(it != childItems.end()) {
				std::lock_guard<std::recursive_mutex> guard(rootNode->sceneMutex);
				childItems.erase(it);
				rootNode->bubbleItem(const_cast<std::shared_ptr<SceneItem>&>(item));
			}
		});
	}
}

SceneGroup::~SceneGroup() {
	if(childGroups != nullptr) {
		delete childGroups;
	}
}
