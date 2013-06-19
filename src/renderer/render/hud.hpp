#pragma once

#include <vector>
#include <memory>
#include <mutex>

#include <glm/glm.hpp>

#include "gltext.hpp"

namespace render {
	struct HUDItem {
		glm::vec3 colour;
		glm::uvec2 position;
		unsigned short pointSize;
		std::string text;

		HUDItem() : colour{1.0f, 1.0f, 1.0f}, pointSize(14) {
		}

		bool operator==(const HUDItem& other) const {
			return other.text == text && other.position == position && other.pointSize == pointSize;
		}
	};

	class HUD {
	 private:
		static std::unique_ptr<HUD> instance;

		std::mutex itemsMutex;

		std::vector<std::shared_ptr<HUDItem>> items;
		gltext::Font font;

		HUD();

	 public:
		HUD(const HUD&) = delete;
		HUD& operator=(const HUD&) = delete;

		static inline HUD& getInstance();

		inline void addItem(std::shared_ptr<HUDItem>& hudItem);
		inline void removeItem(std::shared_ptr<HUDItem>& it);

		inline void setViewPortSize(unsigned int width, unsigned int height);

		void render();
	};

	HUD& HUD::getInstance() {
		if(!instance) {
			instance.reset(new HUD);
		}

		return *instance;
	}

	void HUD::addItem(std::shared_ptr<HUDItem>& item) {
		std::lock_guard<std::mutex> lock(itemsMutex);
		items.push_back(item);
	}

	void HUD::removeItem(std::shared_ptr<HUDItem>& it) {
		std::lock_guard<std::mutex> lock(itemsMutex);
		items.erase(std::find(items.begin(), items.end(), it));
	}

	void HUD::setViewPortSize(unsigned int width, unsigned int height) {
		font.setDisplaySize(width, height);
	}
}