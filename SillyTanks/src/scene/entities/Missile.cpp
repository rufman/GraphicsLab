/*
 * Missile.cpp
 *
 *  Created on: 16.05.2012
 *      Author: gregory
 */
// Class declaration include
#include "Missile.hpp"

// Includes
#include "../../common/GLIncludes.hpp"
#include "../../common/Utils.hpp"

namespace game_space {

Missile::Missile(Scene &scene, float size) :
		Drawable(scene), _size(size), _counter(0) {
	// @ Test
	// draw a test target
}

Missile::~Missile() {
}

void Missile::draw() const {
	glShadeModel(
			_renderingParameters.shadeMode == RenderingParameters::FLAT ?
					GL_FLAT : GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK,
			_renderingParameters.drawMode == RenderingParameters::WIREFRAME ?
					GL_LINE : GL_FILL);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glTranslatef(_position.x, _position.y, _position.z);

	float materialAmbient[3] = { 0.4, 0.1, 0.1 };
	float materialDiffuse[3] = { 0.6, 0.2, 0.2 };
	float materialSpecular[3] = { 0.8, 0.4, 0.4 };
	float materialEmission[3] = { 0.2, 0.0, 0.0 };
	int shininess = 50;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, materialEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glutSolidCube (_size);
	//glutSolidCylinder( _size/2.0, _size * 2.0, 50, 10);
	glutSolidSphere(_size, 10, 10);

	glPopMatrix();
}

void Missile::setPosition(const Point &position) {
	_position = position;
}

void Missile::setVelocity(const Vector3D &velocity) {
	_velocity = velocity;
}

void Missile::move(float seconds) {
	//ballistics of a projectile
	Vector3D toTarget = Vector3D((50 - _position.x), (-_position.y),
			(50 - _position.z));
	Utils::normalize(toTarget);

	if (_counter > 65) {
		_position.x += _velocity.x * seconds + toTarget.x;
		_position.y += _velocity.y * seconds + toTarget.y;
		_position.z += _velocity.z * seconds + toTarget.z;
	} else {
		_position.x += _velocity.x * seconds * 5;
		_position.y += _velocity.y * seconds * 5;
		_position.z += _velocity.z * seconds * 5;
	}

	_counter++;

	//_velocity.y += GRAVITATIONAL_ACCELERATION*seconds;
}

}

