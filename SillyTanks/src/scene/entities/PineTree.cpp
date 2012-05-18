#include "PineTree.hpp"

namespace game_space {

PineTree::PineTree(Scene & scene) :
		PLYModel(scene) {
	load(PINETREE_MODEL);
}

PineTree::~PineTree() {
}

void PineTree::draw() const {
	glPushMatrix();
	glTranslatef(_position.x, _position.y, _position.z);
	PLYModel::draw();
	glPopMatrix();
}

void PineTree::setPosition(Point &position) {
	_position = position;
}

}
