/*
 * fog.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "fog.hpp"

// Includes
#include "defs.hpp"
#include "types.hpp"
#include "scene.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN


Fog::Fog(Scene& scene) :
	_scene(scene) {
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.005);
	glHint(GL_FOG_HINT, GL_NICEST);
}


Fog::~Fog() {
}


void Fog::apply() {
	if(_scene.isMapDebugOn()) {
		return;
	}
	Color3f color = _scene.getBackgroundColor();
	GLfloat fogColor[4] = {color.r, color.g, color.b, 1.0};
	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR, fogColor);
}


GRAPHICSLAB_NAMESPACE_END
