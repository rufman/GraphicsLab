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

	if ( _counter < 65 ){
		Utils::applyGLRotation( Vector3D( 1, 0, 0), _velocity );
	}
	else{
		Utils::applyGLRotation( Vector3D( 1, 0, 0), _toTarget );
	}
	glRotatef(90,0,1,0);
	/*
	float materialAmbient[3] = { 0.1, 0.1, 0.1 };
	float materialDiffuse[3] = { 0.2, 0.2, 0.2 };
	float materialSpecular[3] = { 0.2, 0.4, 0.4 };
	float materialEmission[3] = { 0.1, 0.1, 0.1 };
	int shininess = 50;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, materialEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	*/

	GLUquadricObj *quadObj = gluNewQuadric();
	gluCylinder(quadObj, _size/2.0, _size/2.0, _size*4.0, 10, 10);

	glBegin(GL_TRIANGLES);
	glVertex3f(0,-_size,0);
	glVertex3f(0, 0, _size);
	glVertex3f(0, _size, 0);

	glVertex3f(-_size,0,0);
	glVertex3f(_size, 0, 0);
	glVertex3f(0, 0, _size);
	glEnd();

	glutSolidCone( _size/2.0, 0, 10,10);

	glPushMatrix();
	glTranslatef( 0, 0, _size*4.0);
	glutSolidSphere( _size/2.0, 10, 10 );
	glPopMatrix();

	glTranslatef(-_position.x,- _position.y,- _position.z);
	glRotatef(-90,0,1,0);

	glPopMatrix();
}

void Missile::setPosition(const Point &position) {
	_position = position;
}

void Missile::setVelocity(const Vector3D &velocity) {
	_velocity = velocity;
}

void Missile::setTargetPosition(const Point targetPosition)
{
	_targetPosition = targetPosition;
}

void Missile::move(float seconds) {
	//ballistics of a projectile
	_toTarget = Vector3D((_targetPosition.x - _position.x), (_targetPosition.y-_position.y), (_targetPosition.z - _position.z));
	Utils::normalize(_toTarget);

	if (_counter > 65) {
		_position.x += _velocity.x * seconds + _toTarget.x;
		_position.y += _velocity.y * seconds + _toTarget.y;
		_position.z += _velocity.z * seconds + _toTarget.z;
	} else {
		_position.x += _velocity.x * seconds*50;
		_position.y += _velocity.y * seconds *50;
		_position.z += _velocity.z * seconds*50;
	}

	_counter++;

	//_velocity.y += GRAVITATIONAL_ACCELERATION*seconds;
}

}

