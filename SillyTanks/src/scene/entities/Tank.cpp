/*
 * tank.cpp
 * This class is the base class to create a new tank.
 */

#include "Tank.hpp"

//common includes
#include "../../common/Utils.hpp"
#include "../../common/GLIncludes.hpp"
#include "../../common/Drawable.hpp"

//scene includes
#include "../../scene/Scene.hpp"
#include "../../scene/Terrain.hpp"


GAME_NAMESPACE_BEGIN

Tank::Tank(Scene &scene ) :Drawable( scene ) {}

Tank::~Tank() {}

void Tank::reset() {
	_velocity = Vector3D( 0.0, 0.0, -1.0 );
	_direction = 0;
}

void Tank::draw() const
{

}

Point Tank::getMuzzlePosition() const
{
	return _turret->getMuzzlePosition(_position);
}


float Tank::getShootingPower() const {
	return _turret->getPower();
}

void Tank::setShootingPower(float power) {
	_turret->setPower(power);
}

float Tank::getElevation() const
{
	return _turret->getElevation();
}

void Tank::setElevation(float elevation) {
	_turret->setElevation(elevation);
}

float Tank::getAzimuth() const
{
	return _turret->getAzimuth();
}

void Tank::setAzimuth(float azimuth) {
	_turret->setAzimuth(azimuth);
}

void Tank::move(float speed) {
	_speed = speed;
}

float Tank::getDirection() const
{
	return _direction;
}
void Tank::setDirection(float angle) {
	_velocity = Utils::rotate( angle, Vector3D( 0.0, 0.0, -1.0 ), Vector3D( 0.0, 1.0, 0.0 ) );
	_direction = angle;
}

void Tank::update(float seconds) {

	_position.x += _velocity.x*_speed*seconds;
	_position.y = _scene.getTerrain().getHeight(_position);
	_position.z += _velocity.z*_speed*seconds;
	_speed = 0;
}

LookAt Tank::getLookAt() const
{

	Point from, to;
	Vector3D up = _scene.getTerrain().getNormal(_position);
	const Vector3D &velocity = Utils::rotate(_turret->getAzimuth(),Vector3D(0,0,-1),Vector3D(0,1,0));
	Vector3D direction = velocity;
	Utils::normalize( direction );
	to = _position;
	from.x = to.x - direction.x*10;
	from.z = to.z - direction.z*10;
	from.y = 10;

	return LookAt( from, to, up );
}

Point Tank::getPosition() const
{
	return _position;
}

GAME_NAMESPACE_END
