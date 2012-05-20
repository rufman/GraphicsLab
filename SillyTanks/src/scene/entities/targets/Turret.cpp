/**
 * turret.cpp
 * This class creates a turret for the tank.
 */

// Class declaration include
#include "Turret.hpp"

// common includes
#include "../../../common/GLIncludes.hpp"
#include "../../../common/Utils.hpp"
#include "../../Scene.hpp"
#include "../../Terrain.hpp"

#include <cmath>

namespace game_space {

const float Turret::_baseWidth = 1.0f;

Turret::Turret(Scene &scene) :
		Drawable(scene) {
	_turret = new PLYModel(_scene);
	_turret->load(SMALLTANK_TURRET_MODEL);
	reset();
}

Turret::~Turret() {
}

void Turret::reset() {
	_elevation = 120;
	_azimuth = -85;
	_power = 0.5;
	_position = Point(0, 0, 0, 1);
}

float Turret::getElevation() const {
	return _elevation;
}

void Turret::setElevation(float elevation) {
	//stop the turret from pointing to strange angles.
	if(elevation < 210 && elevation > 120)
	{
		_elevation = elevation;
	}
}

float Turret::getAzimuth() const {
	return _azimuth;
}

void Turret::setAzimuth(float azimuth) {
	_azimuth = azimuth;
}

float Turret::getPower() const {
	return _power;
}

void Turret::setPower(float power) {
	_power = power;
}

Point Turret::getMuzzlePosition() const {
	Point muzzlePosition = _position;
	muzzlePosition.x += -_baseWidth/8-_baseWidth * std::cos(Utils::toRadian(_elevation)) * std::sin(Utils::toRadian(-_azimuth));
	muzzlePosition.y += 2.75 + _baseWidth * std::sin(Utils::toRadian(_elevation));
	muzzlePosition.z += -_baseWidth * std::cos(Utils::toRadian(_elevation)) * std::cos(Utils::toRadian(-_azimuth));

	return muzzlePosition;
}

void Turret::setPosition(Point position)
{
	_position = position;
}

Point Turret::getPosition() const
{
	return _position;
}

void Turret::draw() const {
	// Cannon surface material
	float materialAmbient[3] = { 0.1, 0.1, 0.1 };
	float materialDiffuse[3] = { 0.2, 0.2, 0.2 };
	float materialSpecular[3] = { 0.4, 0.4, 0.4 };
	float materialEmission[3] = { 0.0, 0.0, 0.0 };
	int shininess = 10;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, materialEmission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glTranslatef(_position.x, _position.y, _position.z);

	//a little bit higher over the ground because otherwise the turret is inside of the chassis
	glTranslatef(0, 1.5, 0);

	Utils::applyGLRotation(Vector3D(0.0f, 1.0f, 0.0f), _scene.getTerrain().getNormal(_position));

	//counter intuitive again (see Utils::applyGlRotation
	glRotatef(-_azimuth,0,1,0);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0,-0.5,0);
	glScalef(_baseWidth*2.5 , _baseWidth*2.5 , _baseWidth *2.5);

	_turret->setRenderingParameters(_renderingParameters);
	_turret->draw();
	glRotatef(90, 1, 0, 0);

	glTranslatef(0,0.25,-0.3);

	//counter intuitive again
	glRotatef(_elevation+180, 1.0, 0.0, 0.0);
	GLUquadric *quadric = gluNewQuadric();
	gluCylinder(quadric, _baseWidth / 8, _baseWidth / 8, _baseWidth, 20, 20);
	gluDeleteQuadric(quadric);
}
}
