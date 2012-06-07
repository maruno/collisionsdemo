/*
 * SceneItem.cpp
 *
 *      Author: Marcel Veltman
 */

#include "scene/SceneItem.hpp"

scene::SceneItem::SceneItem(SceneItem* parent) {
	children = new std::list<SceneItem*>();
	if(parent != nullptr) {
		parent->children->push_back(this);
	}
};

scene::SceneItem::~SceneItem() {
	children->clear();
	delete children;
}

void scene::SceneItem::updateScene() {

}

void scene::SceneItem::drawScene() {

}

glm::vec3 scene::SceneItem::getLocation() {
	return location;
}
