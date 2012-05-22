/**
 * sun.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */


// Class declaration include
#include "sun.hpp"

// Includes
#include "scene.hpp"
#include "map.hpp"
#include "directionalLight.hpp"
#include "utils.hpp"
#include <cmath>


GRAPHICSLAB_NAMESPACE_BEGIN

Map* Sun::_map;
float Sun::_angle;
DirectionalLight* Sun::_light;

Sun::Sun(Scene& scene, Map* map) :
	_scene(scene) {
	_map = map;
	_angle = 30.0;
    _light = new DirectionalLight(_scene);
    _light->setAmbient(Color(0.3, 0.3, 0.3));
    _light->setDiffuse(Color(1.0, 1.0, 1.0));
    _light->setSpecular(Color(0.0, 0.0, 0.0));
    _light->turnOn(true);
	_texture = new TGATexture("data/textures/sun.tga");
	_texture->init();
}

Sun::~Sun() {
	delete _light;
	delete _texture;
}

void Sun::apply() {
	_light->apply();
}

void Sun::updatePosition() {
	if(!ReflectionTexture::isDrawing()) {
		_light->setPosition(Point(_map->getMapWidth() / 2,
				                  100000.0 * cos(Utils::toRadian(_angle)),
				                  (_map->getMapHeight() / 2) + (100000.0 * sin(Utils::toRadian(_angle)))));
	} else {
		_light->setPosition(Point(_map->getMapWidth() / 2,
				                  -100000.0 * cos(Utils::toRadian(_angle)),
				                  (_map->getMapHeight() / 2) + (100000.0 * sin(Utils::toRadian(_angle)))));
	}
}

void Sun::draw() {
	glPushMatrix();

	_map->applyReflectionTransformation();

	glDisable(GL_DEPTH);
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor3f(255, 255, 255);

	_texture->setActive(true);

	glRotated(_angle, 1.0, 0.0, 0.0);
	glTranslated(_map->getMapWidth() / 2, 10000.0, _map->getMapHeight() / 2);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(-2000.0, 0.0, -2000.0);

	glTexCoord2f(0.0, 256.0);
	glVertex3f(-2000.0, 0.0,  2000.0);

	glTexCoord2f(256.0, 256.0);
	glVertex3f( 2000.0, 0.0,  2000.0);

	glTexCoord2f(256.0, 0.0);
	glVertex3f( 2000.0, 0.0, -2000.0);

	glEnd();

	_texture->setActive(false);

	glDisable(GL_BLEND);
	glEnable(GL_FOG);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH);

	_map->removeReflectionTransformation();

	glPopMatrix();
}

void Sun::setAngle(float angle) {
	_angle = angle;
}

float Sun::getAngle() {
	return _angle;
}


GRAPHICSLAB_NAMESPACE_END


