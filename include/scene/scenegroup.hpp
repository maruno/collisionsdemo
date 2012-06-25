#ifndef SCENEGROUP_HPP
#define SCENEGROUP_HPP

#include <array>
#include <list>
#include <memory>
#include <functional>

#include <glm/glm.hpp>

#include "scene/sceneitem.hpp"

#include "scene/collisiondetection/AxisAlignedBoundingCuboid.hpp"

namespace scene {
	/**
	 * Scene graph group node.
	 *
	 * @author Michel Bouwmans
	 */
	class SceneGroup final {
		private:
			std::unique_ptr<std::array<SceneGroup, 8>> childGroups;
			std::list<std::unique_ptr<SceneItem>> childItems;

			std::unique_ptr<collisiondetection::AxisAlignedBoundingCuboid> constraints;

			void addOctreeLayers(unsigned int levels);
		public:
			SceneGroup() = default;

			/**
			 * Constructor for the world node.
			 *
			 * @param octreeLevels Number of octree levels that should be used.
			 */
			SceneGroup(unsigned int octreeLevels, scene::collisiondetection::AxisAlignedBoundingCuboid& myConstraints);

			/**
			 * Call visitation-function on all @ref SceneItem childs in the scene graph.
			 * 
			 * 
			 */
			void visitScene(std::function<void(std::unique_ptr<SceneItem>&)> visitation);

			/**
			 * Add a @ref SceneItem child to this node.
			 *
			 * @param child Unique pointer to the @ref SceneItem child. The scene graph takes ownership.
			 */
			inline void addItem(std::unique_ptr<SceneItem> child);
	};

	void SceneGroup::addItem(std::unique_ptr<SceneItem> child) {
		childItems.push_back(std::move(child));
	}
}

#endif // SCENEGROUP_HPP
