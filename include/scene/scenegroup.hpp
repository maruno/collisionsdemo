#ifndef SCENEGROUP_HPP
#define SCENEGROUP_HPP

#include <array>
#include <list>
#include <memory>

#include "scene/sceneitem.hpp"

namespace scene {
	class SceneGroup final {
		private:
			std::unique_ptr<std::array<SceneGroup, 8>> childGroups;
			std::list<std::unique_ptr<SceneItem>> childItems;
			
			void addOctreeLayers(unsigned int levels);
		public:
			SceneGroup() = default;
			SceneGroup(unsigned int octreeLevels);

			void updateScene();
			void drawScene();
			
			inline void addItem(std::unique_ptr<SceneItem> child);
	};
	
	void SceneGroup::addItem(std::unique_ptr<SceneItem> child) {
		childItems.push_back(std::move(child));
	}
}

#endif // SCENEGROUP_HPP
