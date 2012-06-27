#include "scene/scenegroup.hpp"

#include <algorithm>
#include <tuple>
#include <functional>
#include <cassert>

#include "scene/sceneitem.hpp"
#include "scene/collisiondetection/collisiondetector.hpp"

using namespace scene;

typedef collisiondetection::AxisAlignedBoundingCuboid AABB;
typedef std::tuple<glm::vec3, glm::vec3> Diagonal;

SceneGroup::SceneGroup(unsigned int octreeLevels, collisiondetection::AxisAlignedBoundingCuboid myConstraints)
	: constraints(new AABB(myConstraints)) {
	addOctreeLayers(octreeLevels);
}

void SceneGroup::addOctreeLayers(unsigned int levels) {
	if(levels != 0) {
		childGroups.reset(new std::array<SceneGroup,8>);

		//Calculate and set constraints for all childGroups
		float width = (constraints->getMaxX() - constraints->getMinX()) / 2.0f;
		float height = (constraints->getMaxY() - constraints->getMinY()) / 2.0f;
		float depth = (constraints->getMaxZ() - constraints->getMinZ()) / 2.0f;

		glm::vec3 childMinimum(constraints->getMinX(), constraints->getMinY(), constraints->getMaxZ());
		glm::vec3 childMaximum(constraints->getMinX() + width, constraints->getMinY() + height, constraints->getMaxZ() - depth);

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
	std::for_each(childItems.begin(), childItems.end(), visitation);
	
	if(childGroups != nullptr) {
		std::for_each(childGroups->begin(), childGroups->end(),
		[&](SceneGroup& child) {
			child.visitScene(visitation);
		});
	}
}

void SceneGroup::bubbleItem(std::unique_ptr<SceneItem> item) {
	if(childGroups != nullptr) {
		std::function<bool(SceneGroup&)> itemInGroup = [&item](SceneGroup& child) {
			return collisiondetection::intersects(*(child.constraints), item->getBounds());
		};
		
		auto viableGroup = std::find_if(childGroups->begin(), childGroups->end(), itemInGroup);
		
		assert(viableGroup != childGroups->end());
		
		if(viableGroup+1 != childGroups->end()) {
			auto otherGroup = std::find_if(viableGroup+1, childGroups->end(), itemInGroup);
			
			if(otherGroup != childGroups->end()) {
				childItems.push_back(std::move(item));
				return;
			}
		}
		
		viableGroup->bubbleItem(std::move(item));
	}
	
	childItems.push_back(std::move(item));
}
