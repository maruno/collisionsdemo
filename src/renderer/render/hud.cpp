#include "hud.hpp"

using namespace render;

std::unique_ptr<HUD> HUD::instance;

HUD::HUD() {
	font = gltext::Font("font.ttf", 32);
	font.cacheCharacters("1234567890GAMEOVERscore:");

	items.reserve(10);
}

void HUD::render() {
	std::lock_guard<std::mutex> lock(itemsMutex);
	for(std::shared_ptr<HUDItem>& item : items) {
		font.setPenPosition(item->position.x, item->position.y);
		font.setPenColor(item->colour.r, item->colour.g, item->colour.b);
		font.setPointSize(item->pointSize);
		font.draw(item->text);
	}
}