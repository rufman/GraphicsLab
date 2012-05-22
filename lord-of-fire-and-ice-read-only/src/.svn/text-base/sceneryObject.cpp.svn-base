/*
 * sceneryObject.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "sceneryObject.hpp"

// Includes
#include "defs.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN


SceneryObject::SceneryObject(Scene& scene, SceneryObjectManager* manager, int x, int y, float altitude, int type) :
	Drawable(scene, x, y, altitude),
	_animationPosition(0.0),
	_manager(manager),
	_type(type) {
	_manager->addObject(this);
}


SceneryObject::~SceneryObject() {
}


void SceneryObject::draw(int quality) {
	_manager->draw(_x, _y, _altitude, _type, quality, _animationPosition);
}


GRAPHICSLAB_NAMESPACE_END
