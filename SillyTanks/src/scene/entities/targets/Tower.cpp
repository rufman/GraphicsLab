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

#include "../../AI/TowerAI.hpp"


namespace game_space {

Tower::Tower(Scene &scene,bool isAIControlled) :Target( scene,Target::TOWER ),_isAIControlled(isAIControlled) {
	//the ai must know the tank to be able to control it
	std::vector<Message*>* messageSubbus = _scene.getMessageBus()->addNewClient();
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

	_scene.getSoundEngine().playMuzzleSound();
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

	_scene._bullets.push_back(bullet);
}

void Tower::fireMissile() {
		Missile* missile = new Missile(_scene);

		missile->setPosition(getMuzzlePosition());

		Vector3D velocity(
				-getShootingPower()
						* std::cos(Utils::toRadian(getElevation()))
						* std::sin(Utils::toRadian(-getAzimuth())),
				getShootingPower()
						* std::sin(Utils::toRadian(getElevation())),
				-getShootingPower()
						* std::cos(Utils::toRadian(getElevation()))
						* std::cos(Utils::toRadian(-getAzimuth())));
		missile->setVelocity(velocity);

		_scene.getSoundEngine().playExplosionSound();
		_scene._missiles.push_back(missile);
}

Tower::SELECTEDWEAPON Tower::getSelectedWeapon()
{
	return _selectedWeapon;
}

void Tower::setSelectedWeapon(SELECTEDWEAPON weapon)
{
	_selectedWeapon = weapon;
}

} /* namespace game_space */
