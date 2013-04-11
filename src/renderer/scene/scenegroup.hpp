#ifndef SCENEGROUP_HPP
#define SCENEGROUP_HPP

#include <array>
#include <list>
#include <memory>
#include <functional>
#include <mutex>

#include <glm/glm.hpp>

#include "renderer/scene/sceneitem.hpp"

#include "collisiondetection/axisalignedboundingbox.hpp"

namespace scene {
	class SceneManager;
	class SceneGroup;

	/**
	 * Scene graph group node.
	 *
	 * @author Michel Bouwmans
	 */
	class SceneGroup final {
		friend class SceneManager;

	 private:
		std::array<SceneGroup, 8>* childGroups;
		std::list<std::shared_ptr<SceneItem>> childItems;

		std::shared_ptr<collisiondetection::AxisAlignedBoundingBox> constraints;

		static SceneGroup* rootNode;
		static std::recursive_mutex sceneMutex;

		SceneGroup* parent;

		unsigned int sceneDepth;

		void addOctreeLayers(unsigned int levels, unsigned int myDepth);

		/**
		 * Add an @ref SceneItem to the SceneGroup and adds octreelevels if there are lots of items
		 *
		 * \param item Unique pointer to the @ref SceneItem child. The scene graph takes ownership.
		 */
		void addItem(std::shared_ptr<SceneItem>& item);

		/**
		 * Try to relocate the suspiciously placed @ref SceneItem child at a later time.
		 *
		 * \param item Child to try and relocate later
		 */
		void tryRelocationLater(std::shared_ptr<SceneItem> item);
	 public:
		SceneGroup();

		/**
		 * Constructor for the root node.
		 *
		 * @param octreeLevels Number of octree levels that should be used.
		 * @param myConstraints Constraining box of this scene graph.
		 */
		SceneGroup(unsigned int octreeLevels, collisiondetection::AxisAlignedBoundingBox myConstraints);

		/**
		 * Call visitation-function on all @ref SceneItem childs in the scene graph.
		 *
		 * @param visitation Function to call.
		 */
		void visitScene(std::function<void(std::shared_ptr<SceneItem>&)> visitation);

		/**
		 * Call visitation-function on all @ref SceneGroup childs in the scene graph.
		 *
		 * @param visitation Function to call.
		 */
		void visitGroups(std::function<void(SceneGroup&)> visitation);

		/**
		 * Call visitation-function on all @ref SceneGroup parents in the scene graph.
		 *
		 * \param visitation Function to call.
		 */
		void visitParentGroups(std::function<void(SceneGroup&)> visitation);

		/**
		 * Bubble a @ref SceneItem to the correct group in the scene graph.
		 *
		 * @param child Unique pointer to the @ref SceneItem child. The scene graph takes ownership.
		 */
		void bubbleItem(std::shared_ptr<SceneItem>& item);


		~SceneGroup();
	};
}

#endif // SCENEGROUP_HPP
