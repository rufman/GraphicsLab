/*
 * tank.cpp
 * This class is the base class to create a new tank.
 */

#include "Tank.hpp"

//common includes
#include "../../../common/Utils.hpp"
#include "../../../common/GLIncludes.hpp"
#include "../../../common/Drawable.hpp"
#include "../../../common/Types.hpp"
#include "../../../common/SoundEngine.hpp"

//std includes
#include <math.h>
#include <iostream>

//scene includes
#include "../../../scene/Scene.hpp"
#include "../../../scene/Terrain.hpp"

//projectile includes
#include "../projectiles/Bullet.hpp"
#include "../projectiles/Missile.hpp"
#include "../projectiles/Robot.hpp"

//AI includes
#include "../../AI/TankAI.hpp"
#include "../../AI/MessageBus.hpp"

//particle engine includes

namespace game_space {

Tank::Tank(Scene &scene, bool isAIControlled) :
		Target(scene, Target::TANK), _velocity(Vector3D(0.0, 0.0, 1.0)), _direction(
				0), _isAIControlled(isAIControlled) {

	if (isAIControlled) {
		//the ai must know the tank to be able to control it
		std::vector<Message*>* messageBus =
				_scene.getMessageBus()->addNewClient();
		_controllingAI = new TankAI(scene, messageBus);
		_controllingAI->_tank = this;
	}

	_tankSmokeParticleEngine = new ParticleEngine<Smoke>(
			_scene.getCurrentlyActiveCamera(), 20);
	_tankSmokeParticleEngine->setStartAcceleration(Vector3D(0, 0, 0));
	_tankSmokeParticleEngine->setActive(true);
}

Tank::~Tank() {
	delete _tankSmokeParticleEngine;
}

void Tank::reset() {
	_position = _scene.getTerrain().getRandomPointOnMap();
	_velocity = Vector3D(0.0, 0.0, 1.0);
	_direction = 0;
}

void Tank::draw() const {
	_tankSmokeParticleEngine->draw();
}

void Tank::drawShadow(const Point &lightPostion) const {

}
void Tank::setNeighbors() const {

}

Point Tank::getMuzzlePosition() const {
	return _turret->getMuzzlePosition();
}

float Tank::getShootingPower() const {
	return _turret->getPower();
}

void Tank::setShootingPower(float power) {
	_turret->setPower(power);
}

float Tank::getElevation() const {
	return _turret->getElevation();
}

void Tank::setElevation(float elevation) {
	_turret->setElevation(elevation);
}

float Tank::getAzimuth() const {
	return _turret->getAzimuth();
}

void Tank::setAzimuth(float azimuth) {
	_turret->setAzimuth(azimuth);
}

void Tank::move(float speed) {
	_speed = speed;
}

float Tank::getDirection() const {
	return _direction;
}

void Tank::setDirection(float angle) {
	_velocity = Utils::rotate(angle, Vector3D(0.0, 0.0, 1.0),
			Vector3D(0.0, 1.0, 0.0));
	_direction = angle;
}

void Tank::update(float seconds) {
	float averagedHeight = _scene.getTerrain().getHeight(
			Point(_position.x - 1, _position.y, _position.z - 1));
	averagedHeight += _scene.getTerrain().getHeight(
			Point(_position.x + 1, _position.y, _position.z - 1));
	averagedHeight += _scene.getTerrain().getHeight(
			Point(_position.x, _position.y, _position.z + 1));
	averagedHeight = averagedHeight / 3.0;

	Vector3D gravity = Vector3D(0, 1, 0);
	Vector3D realGravity = Vector3D(0, -1, 0);
	Vector3D firstNormal = _scene.getTerrain().getNormal(
			Point(_position.x - 1, _position.y, _position.z - 1));
	Vector3D secondNormal = _scene.getTerrain().getNormal(
			Point(_position.x + 1, _position.y, _position.z - 1));
	Vector3D thirdNormal = _scene.getTerrain().getNormal(
			Point(_position.x, _position.y, _position.z - 1));
	Vector3D normal = Vector3D(
			(firstNormal.x + secondNormal.x + thirdNormal.x) / 3.0,
			(firstNormal.y + secondNormal.y + thirdNormal.y) / 3.0,
			(firstNormal.z + secondNormal.z + thirdNormal.z) / 3.0);

	float angleGravityNormal = acos(Utils::dot(gravity, normal));
	Vector3D relativeGravity = Vector3D(
			angleGravityNormal * 15.0 * (realGravity.x + normal.x),
			angleGravityNormal * (realGravity.y + normal.y),
			angleGravityNormal * 15.0 * (realGravity.z + normal.z));

	Point nextPosition(
			_position.x
					+ (_velocity.x * _speed * seconds
							+ relativeGravity.x * seconds), averagedHeight,
			_position.z
					+ (_velocity.z * _speed * seconds
							+ relativeGravity.z * seconds));

	if (_scene.getTerrain().checkBorder(nextPosition)) {
		_speed = 0;
		//make the gravity move the tank away, otherwise the tank can get stuck
		relativeGravity.x = -relativeGravity.x;
		relativeGravity.z = -relativeGravity.z;
		relativeGravity.y = -relativeGravity.y;
	}

	_position.x += _velocity.x * _speed * seconds + relativeGravity.x * seconds;
	_position.y = averagedHeight;
	_position.z += _velocity.z * _speed * seconds + relativeGravity.z * seconds;

	//reset speed to make it necessary to keep the button pressed
	_speed = 0;

	_tankSmokeParticleEngine->setStartPosition(_position);
	_tankSmokeParticleEngine->update(seconds);
}

LookAt Tank::getLookAt() const {
	Point from, to;

	const Vector3D &velocity = Utils::rotate(_turret->getAzimuth(),
			Vector3D(0, 0, -1), Vector3D(0, 1, 0));
	Vector3D direction = velocity;

	Utils::normalize(direction);
	Vector3D tankShootdirection(
			-getShootingPower() * std::cos(Utils::toRadian(getElevation()))
					* std::sin(Utils::toRadian(-getAzimuth())),
			getShootingPower() * std::sin(Utils::toRadian(getElevation())),
			-getShootingPower() * std::cos(Utils::toRadian(getElevation()))
					* std::cos(Utils::toRadian(-getAzimuth())));

	to.x = _position.x + tankShootdirection.x * 20;
	to.y = _position.y + tankShootdirection.y * 20;
	to.z = _position.z + tankShootdirection.z * 20;

	from.x = _position.x - direction.x * 15;
	from.z = _position.z - direction.z * 15;
	from.y = 10;

	return LookAt(from, to, Vector3D(0, 1, 0));
}

bool Tank::isAIControlled() const {
	return _isAIControlled;
}

TankAI* Tank::getAI() const {
	return _controllingAI;
}

void Tank::fireBullet() {
	Bullet* bullet = new Bullet(_scene);
	bullet->setPosition(getMuzzlePosition());
	float velocityScale = 30;
	Vector3D velocity(
			-velocityScale * getShootingPower()
					* std::cos(Utils::toRadian(getElevation()))
					* std::sin(Utils::toRadian(-getAzimuth())),
			velocityScale * getShootingPower()
					* std::sin(Utils::toRadian(getElevation())),
			-velocityScale * getShootingPower()
					* std::cos(Utils::toRadian(getElevation()))
					* std::cos(Utils::toRadian(-getAzimuth())));
	bullet->setVelocity(velocity);

	_scene._projectiles.push_back(bullet);
	_scene.getSoundEngine().playMuzzleSoundAt(_position.x, _position.y,
			_position.z);
}

void Tank::fireMissile(Point targetPosition) {
	Missile* missile = new Missile(_scene);
	missile->setPosition(getMuzzlePosition());
	missile->setTargetPosition(targetPosition);

	Vector3D velocity(
			-getShootingPower() * std::cos(Utils::toRadian(getElevation()))
					* std::sin(Utils::toRadian(-getAzimuth())),
			getShootingPower() * std::sin(Utils::toRadian(getElevation())),
			-getShootingPower() * std::cos(Utils::toRadian(getElevation()))
					* std::cos(Utils::toRadian(-getAzimuth())));
	missile->setVelocity(velocity);

	_scene._projectiles.push_back(missile);
	_scene.getSoundEngine().playExplosionSoundAt(_position.x, _position.y,
			_position.z);
}

void Tank::fireRobot() {
	Robot* robot = new Robot(_scene);
	robot->setPosition(getMuzzlePosition());

	int anim = rand()%3;
	std::string animFile;

	switch(anim)
	{
	case 0:
	{
		animFile = ROBOT_ANIMATION1;
		break;
	}
	case 1:
	{
		animFile = ROBOT_ANIMATION2;
		break;
	}
	case 2:
	{
		animFile = ROBOT_ANIMATION3;
		break;
	}
	}

	robot->loadAnimation(animFile);
	float velocityScale = 30;
	Vector3D velocity(
			-velocityScale * getShootingPower()
					* std::cos(Utils::toRadian(getElevation()))
					* std::sin(Utils::toRadian(-getAzimuth())),
			velocityScale * getShootingPower()
					* std::sin(Utils::toRadian(getElevation())),
			-velocityScale * getShootingPower()
					* std::cos(Utils::toRadian(getElevation()))
					* std::cos(Utils::toRadian(-getAzimuth())));
	robot->setVelocity(velocity);

	_scene._projectiles.push_back(robot);
	_scene.getSoundEngine().playMuzzleSoundAt(_position.x, _position.y,
			_position.z);
}

Tank::SelectedWeapon Tank::getSelectedWeapon() {
	return _selectedWeapon;
}

void Tank::setSelectedWeapon(SelectedWeapon weapon) {
	_selectedWeapon = weapon;
}

}
