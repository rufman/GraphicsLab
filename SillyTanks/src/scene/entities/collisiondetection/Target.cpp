/*
 * Target.cpp
 */

#include "Target.hpp"

namespace game_space {
Target::Target(Scene &scene, TargetType type) :
		Drawable(scene), _targetType(type), _position(Point(0, 0, 0)) {
	reset();
}

bool Target::collidesWithOtherTarget(Target* target) {
	return false;
}

BoundingBox* Target::getBoundingBox() {
	return _boundingBox;

}
void Target::draw() const {

}
void Target::reset() {
	switch (_targetType) {
	case TANK: {
		_life = SMALLTANK_LIFE;
		_shield = SMALLTANK_SHIELD;
		_amountOfMissiles = SMALLTANK_QTYMISSILES;
		_amountOfRobots = SMALLTANK_QTYROBOTS;
		_reloadingTime = SMALLTANK_RELOADING_TIME;
		break;
	}
	case TOWER: {
		_life = SMALLTOWER_LIFE;
		_shield = SMALLTOWER_SHIELD;
		_amountOfMissiles = SMALLTOWER_QTYMISSILES;
		_amountOfRobots = SMALLTOWER_QTYROBOTS;
		_reloadingTime = SMALLTOWER_RELOADING_TIME;
		break;
	}
	}
}

void Target::setPosition(Point position) {
	_position = position;
}

Point Target::getPosition() const {
	return _position;
}

float Target::getBoundingRadius() {
	return _boundingRadius;
}

void Target::setLife(float life) {
	_life = life;
}
float Target::getLife() {
	return _life;
}

void Target::setShield(float shield) {
	_shield = shield;
}
float Target::getShield() {
	return _shield;
}

void Target::setQtyMissiles(uint qty) {
	_amountOfMissiles = qty;
}
uint Target::getQtyMissiles() {
	return _amountOfMissiles;
}

void Target::setQtyRobots(uint qty) {
	_amountOfRobots = qty;
}
uint Target::getQtyRobots() {
	return _amountOfRobots;
}

void Target::doDamage(float damage) {
	if (_shield > 0) {
		if (_shield - damage > 0) {
			_shield -= damage;
		} else {
			damage -= _shield;
			_shield = 0;
			_life -= damage;

		}
	} else {
		if (_life - damage > 0) {
			_life -= damage;
		} else {
			_life = 0;
		}
	}

}

}

