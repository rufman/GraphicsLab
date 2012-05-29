/*
 * tank.hpp
 * See tank.cpp
 */

#ifndef TANK_HPP_
#define TANK_HPP_

// common includes
#include "../../../common/Drawable.hpp"


#include <math.h>

//entities include
#include "Turret.hpp"
#include "../collisiondetection/Target.hpp"


namespace game_space {

//forward declaration
class TankAI;
class PLYModel;
class LookAt;

//particle engine forward declarations
template <class T>
class ParticleEngine;
class Smoke;
class Tank: public Target {
public:
	enum SelectedWeapon {
		BULLET, MISSILE, ROBOT
	};

	//delay the shooting a bit
	float _reloadingTime;
	float currentTilt;
protected:
	Turret* _turret;

	PLYModel* _chassis;

	/** Ship velocity */
	Vector3D _velocity;

	float _speed;

	float _direction;

	float _baseWidth;

	int _targetId;
	TankAI* _controllingAI;
	bool _isAIControlled;
	bool _firstRun;

	SelectedWeapon _selectedWeapon;

	ParticleEngine<Smoke> *_tankSmokeParticleEngine;

	float _secToShieldReload;


public:
	Tank(Scene &scene,bool isAIControlled);
	virtual ~Tank();
	void reset();

	/** Draw the object to the screen. */
	void draw() const;
	virtual void drawShadow(const Point &lightPostion) const;
	virtual void setNeighbors() const;

	Point getMuzzlePosition() const;
	float getShootingPower() const;
	void setShootingPower(float power);

	float getElevation() const;
	void setElevation(float elevation);

	float getAzimuth() const;
	void setAzimuth(float azimuth);
	void move(float speed);
	void update(float seconds);
	float getDirection() const;
	void setDirection(float angle);
	LookAt getLookAt() const;
	bool isAIControlled() const;
	TankAI* getAI() const;
	void fireBullet();
	void fireMissile(Point targetPosition);
	void fireRobot();

	//only used for a human player
	SelectedWeapon getSelectedWeapon();
	void setSelectedWeapon(SelectedWeapon weapon);

};
}
#endif
