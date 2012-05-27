/*
 * Tower.cpp
 */

#include "Tower.hpp"

//common includes
#include "../../../common/Utils.hpp"
#include "../../../common/GLIncludes.hpp"
#include "../../../common/Drawable.hpp"

#include <math.h>
#include <iostream>

//scene includes
#include "../../../scene/Scene.hpp"
#include "../../../scene/Terrain.hpp"

#include "../projectiles/Bullet.hpp"
#include "../projectiles/Missile.hpp"
#include "../projectiles/Robot.hpp"

#include "../../AI/TowerAI.hpp"
#include "../../AI/MessageBus.hpp"
#include "../../AI/MessageSubBus.hpp"


namespace game_space {

Tower::Tower(Scene &scene,bool isAIControlled) :Target( scene,Target::TOWER ),_isAIControlled(isAIControlled) {
	//the ai must know the tank to be able to control it
	MessageSubBus* messageSubbus = _scene.getMessageBus()->addNewClient();
	_controllingAI = new TowerAI(scene,messageSubbus);
	_controllingAI->_tower = this;
}

Tower::~Tower() {}

void Tower::reset() {
	_position = _scene.getTerrain().getRandomPointOnMap();
}

void Tower::draw() const
{

}

Point Tower::getMuzzlePosition() const
{
	return _turret->getMuzzlePosition();
}


float Tower::getShootingPower() const {
	return _turret->getPower();
}

void Tower::setShootingPower(float power) {
	_turret->setPower(power);
}

float Tower::getElevation() const
{
	return _turret->getElevation();
}

void Tower::setElevation(float elevation) {
	_turret->setElevation(elevation);
}

float Tower::getAzimuth() const
{
	return _turret->getAzimuth();
}

void Tower::setAzimuth(float azimuth) {
	_turret->setAzimuth(azimuth);
}




void Tower::update(float seconds) {

}

LookAt Tower::getLookAt() const
{
	Point from, to;

	const Vector3D &velocity = Utils::rotate(_turret->getAzimuth(),Vector3D(0,0,-1),Vector3D(0,1,0));
	Vector3D direction = velocity;
	Utils::normalize( direction );
	to = _position;
	from.x = to.x - direction.x*10;
	from.z = to.z - direction.z*10;
	from.y = 10;

	return LookAt( from, to, Vector3D(0,1,0) );
}

bool Tower::isAIControlled() const
{
	return _isAIControlled;
}

TowerAI* Tower::getAI() const
{
	return _controllingAI;
}

void Tower::fireBullet() {
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

	Vector3D normal = _scene.getTerrain().getNormal(_position);
	float dot = Utils::dot(Vector3D(0,1,0),normal);
	Vector3D cross = Utils::cross(Vector3D(0,1,0),normal);
	velocity = Utils::rotate(-Utils::toDegree(acos(dot)),velocity,cross);
	bullet->setVelocity(velocity);

	_scene._projectiles.push_back(bullet);
	_scene.getSoundEngine().playMuzzleSound();
}

void Tower::fireMissile(Point targetPosition) {
	Missile* missile = new Missile(_scene);
	missile->setPosition(getMuzzlePosition());
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
	_scene.getSoundEngine().playExplosionSoundAt(_position.x, _position.y,
			_position.z);
}

void Tower::fireRobot() {
	Robot* robot = new Robot(_scene);
	robot->setPosition(getMuzzlePosition());

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
	float dot = Utils::dot(Vector3D(0,1,0),normal);
	Vector3D cross = Utils::cross(Vector3D(0,1,0),normal);
	velocity = Utils::rotate(-Utils::toDegree(acos(dot)),velocity,cross);
	robot->setVelocity(velocity);

	_scene._projectiles.push_back(robot);
	_scene.getSoundEngine().playMuzzleSoundAt(_position.x, _position.y, _position.z);
}

Tower::SelectedWeapon Tower::getSelectedWeapon()
{
	return _selectedWeapon;
}

void Tower::setSelectedWeapon(SelectedWeapon weapon)
{
	_selectedWeapon = weapon;
}

} /* namespace game_space */
