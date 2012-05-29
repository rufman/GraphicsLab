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
#define MAP_SIZE 200;

namespace game_space {

Tank::Tank(Scene &scene, bool isAIControlled) :
		Target(scene, Target::TANK), _velocity(Vector3D(0.0, 0.0, 1.0)), _direction(0), _isAIControlled(isAIControlled),_firstRun(true), _selectedWeapon(Tank::BULLET),_reloadingTime(SMALLTANK_RELOADING_TIME) {


	if (isAIControlled) {
		//the ai must know the tank to be able to control it
		MessageSubBus* messageBus = _scene.getMessageBus()->addNewClient();
		_controllingAI = new TankAI(scene, messageBus);
		_controllingAI->_tank = this;
	}

	_tankSmokeParticleEngine = new ParticleEngine<Smoke>(_scene.getCurrentlyActiveCamera(), 20);
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
	Target::reset();
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

	if(_scene.getSoundEngine().isEngineSoundFinished()){

		float distance = Utils::distance(_scene._playerTank->getPosition(), _position);
		float tmp = distance / MAP_SIZE;
		float distanceVolume = 1 - tmp;
		_scene.getSoundEngine().changeVolumeEngine(distanceVolume*0.5);
		_scene.getSoundEngine().playEngineSoundAt(_position.x, _position.y, _position.z);
	}
	else if(!_isAIControlled){
		_scene.getSoundEngine().setEngineSoundPosition(_position.x,_position.y,_position.z);
	}

}

float Tank::getDirection() const {
	return _direction;
}

void Tank::setDirection(float angle) {
	_velocity = Utils::rotate(angle, Vector3D(0.0, 0.0, 1.0), Vector3D(0.0, 1.0, 0.0));
	_direction = angle;
}

void Tank::update(float seconds) {


	if(_reloadingTime - seconds >= 0)
	{
		_reloadingTime -= seconds;
	}
	else
	{
		_reloadingTime = 0;
	}


	if(_secToShieldReload < 0)
	{
		if(_shield < SMALLTANK_SHIELD)
		{
			_shield+= seconds;
		}
	}
	else
	{
		_secToShieldReload -= seconds;
	}

	float averagedHeight = _scene.getTerrain().getHeight(Point(_position.x - 1, _position.y, _position.z - 1));
	averagedHeight += _scene.getTerrain().getHeight(Point(_position.x + 1, _position.y, _position.z - 1));
	averagedHeight += _scene.getTerrain().getHeight(Point(_position.x, _position.y, _position.z + 1));
	averagedHeight = averagedHeight / 3.0;

	Vector3D gravity = Vector3D(0, 1, 0);
	Vector3D realGravity = Vector3D(0, -1, 0);
	Vector3D firstNormal = _scene.getTerrain().getNormal(Point(_position.x - 1, _position.y, _position.z - 1));
	Vector3D secondNormal = _scene.getTerrain().getNormal(Point(_position.x + 1, _position.y, _position.z - 1));
	Vector3D thirdNormal = _scene.getTerrain().getNormal(Point(_position.x, _position.y, _position.z - 1));
	Vector3D normal = Vector3D((firstNormal.x + secondNormal.x + thirdNormal.x) / 3.0, (firstNormal.y + secondNormal.y + thirdNormal.y) / 3.0, (firstNormal.z + secondNormal.z + thirdNormal.z) / 3.0);

	float angleGravityNormal = acos(Utils::dot(gravity, normal));
	Vector3D relativeGravity = Vector3D(angleGravityNormal * 15.0 * (realGravity.x + normal.x), angleGravityNormal * (realGravity.y + normal.y), angleGravityNormal * 15.0 * (realGravity.z + normal.z));

	Point nextPosition(_position.x + (_velocity.x * _speed * seconds + relativeGravity.x * seconds), averagedHeight, _position.z + (_velocity.z * _speed * seconds + relativeGravity.z * seconds));

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

	const Vector3D &velocity = Utils::rotate(_turret->getAzimuth(), Vector3D(0, 0, -1), Vector3D(0, 1, 0));
	Vector3D direction = velocity;

	Utils::normalize(direction);
	Vector3D tankShootdirection(-getShootingPower() * std::cos(Utils::toRadian(getElevation())) * std::sin(Utils::toRadian(-getAzimuth())), getShootingPower() * std::sin(Utils::toRadian(getElevation())), -getShootingPower() * std::cos(Utils::toRadian(getElevation())) * std::cos(Utils::toRadian(-getAzimuth())));

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

	if(_firstRun){
		_scene.getSoundEngine().playEngineSoundAt(_position.x, _position.y, _position.z);
		_firstRun = false;
	}


	if(_reloadingTime == 0)
	{
	Bullet* bullet = new Bullet(_scene);
	bullet->setPosition(getMuzzlePosition());
	bullet->_projectileOwner = this;
	float velocityScale = 30;

	Vector3D velocity(-velocityScale * getShootingPower() * std::cos(Utils::toRadian(getElevation())) * std::sin(Utils::toRadian(-getAzimuth())), velocityScale * getShootingPower() * std::sin(Utils::toRadian(getElevation())), -velocityScale * getShootingPower() * std::cos(Utils::toRadian(getElevation())) * std::cos(Utils::toRadian(-getAzimuth())));
	Vector3D normal = _scene.getTerrain().getNormal(_position);
	float dot = Utils::dot(Vector3D(0, 1, 0), normal);
	Vector3D cross = Utils::cross(Vector3D(0, 1, 0), normal);
	currentTilt = -Utils::toDegree(acos(dot));
	velocity = Utils::rotate(currentTilt, velocity, cross);
	bullet->setVelocity(velocity);

	_scene._projectiles.push_back(bullet);


	float distance = Utils::distance(_scene._playerTank->getPosition(), _position);

	float tmp = distance / MAP_SIZE;
	float distanceVolume = 1 - tmp;
	_scene.getSoundEngine().changeVolumeMuzzle(distanceVolume*0.5);
	_scene.getSoundEngine().playMuzzleSoundAt(_position.x, _position.y, _position.z);

	_reloadingTime = SMALLTANK_RELOADING_TIME;
	}
}

void Tank::fireMissile(Point targetPosition) {
	if (_amountOfMissiles > 0 && _reloadingTime == 0) {
		Missile* missile = new Missile(_scene);
		missile->setPosition(getMuzzlePosition());
		missile->_projectileOwner = this;
		missile->setTargetPosition(targetPosition);

		Vector3D velocity(
				-getShootingPower() * std::cos(Utils::toRadian(getElevation()))
						* std::sin(Utils::toRadian(-getAzimuth())),
				getShootingPower() * std::sin(Utils::toRadian(getElevation())),
				-getShootingPower() * std::cos(Utils::toRadian(getElevation()))
						* std::cos(Utils::toRadian(-getAzimuth())));

		Vector3D normal = _scene.getTerrain().getNormal(_position);
		float dot = Utils::dot(Vector3D(0,1,0),normal);
		Vector3D cross = Utils::cross(Vector3D(0,1,0),normal);
		velocity = Utils::rotate(-Utils::toDegree(acos(dot)),velocity,cross);
		missile->setVelocity(velocity);

		_scene._projectiles.push_back(missile);

		float distance = Utils::distance(_scene._playerTank->getPosition(), _position);
		float tmp = distance / MAP_SIZE;
		float distanceVolume = 1 - tmp;

		_scene.getSoundEngine().changeVolumeExplosion(distanceVolume*0.5);
		_scene.getSoundEngine().playExplosionSoundAt(_position.x, _position.y,
				_position.z);

		_amountOfMissiles--;
		_reloadingTime = SMALLTANK_RELOADING_TIME;
	}
}

void Tank::fireRobot() {
	if(_amountOfRobots > 0 && _reloadingTime == 0)
	{
	Robot* robot = new Robot(_scene);
	robot->setPosition(getMuzzlePosition());
	robot->_projectileOwner = this;

	int anim = rand() % 3;
	std::string animFile;

	switch (anim) {
	case 0: {
		animFile = ROBOT_ANIMATION1;
		break;
	}
	case 1: {
		animFile = ROBOT_ANIMATION2;
		break;
	}
	case 2: {
		animFile = ROBOT_ANIMATION3;
		break;
	}
	}

	robot->loadAnimation(animFile);
	float velocityScale = 30;
	Vector3D velocity(-velocityScale * getShootingPower() * std::cos(Utils::toRadian(getElevation())) * std::sin(Utils::toRadian(-getAzimuth())), velocityScale * getShootingPower() * std::sin(Utils::toRadian(getElevation())), -velocityScale * getShootingPower() * std::cos(Utils::toRadian(getElevation())) * std::cos(Utils::toRadian(-getAzimuth())));
	Vector3D normal = _scene.getTerrain().getNormal(_position);
	float dot = Utils::dot(Vector3D(0, 1, 0), normal);
	Vector3D cross = Utils::cross(Vector3D(0, 1, 0), normal);
	velocity = Utils::rotate(-Utils::toDegree(acos(dot)), velocity, cross);
	robot->setVelocity(velocity);

	_scene._projectiles.push_back(robot);
	_scene.getSoundEngine().playRayGunSoundAt(_position.x, _position.y, _position.z);
	_amountOfRobots--;
	_reloadingTime = SMALLTANK_RELOADING_TIME;
	}
}

Tank::SelectedWeapon Tank::getSelectedWeapon() {
	return _selectedWeapon;
}

void Tank::setSelectedWeapon(SelectedWeapon weapon) {
	_selectedWeapon = weapon;
}

}
