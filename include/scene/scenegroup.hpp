#ifndef SCENEGROUP_HPP
#define SCENEGROUP_HPP

#include <array>
#include <list>
#include <memory>
#include <functional>

#include <glm/glm.hpp>

#include "scene/sceneitem.hpp"

#include "scene/collisiondetection/axisalignedboundingcuboid.hpp"

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
			SceneGroup(unsigned int octreeLevels, scene::collisiondetection::AxisAlignedBoundingCuboid myConstraints);

			/**
			 * Call visitation-function on all @ref SceneItem childs in the scene graph.
			 * 
			 * 
			 */
			void visitScene(std::function<void(std::unique_ptr<SceneItem>&)> visitation);

			/**
			 * Bubble a @ref SceneItem to the correct group in the scene graph.
			 *
			 * @param child Unique pointer to the @ref SceneItem child. The scene graph takes ownership.
			 */
			void bubbleItem(std::unique_ptr<SceneItem> item);
	};
}

#endif // SCENEGROUP_HPP
