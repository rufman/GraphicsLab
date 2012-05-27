/*
 * Tower.h
 */

#ifndef TOWER_H_
#define TOWER_H_

#include "../collisiondetection/Target.hpp"
#include "../../../common/PLYModel.hpp"
#include "Turret.hpp"
#include "../../../common/Camera3D.hpp"

namespace game_space {

//forward declaration
class TowerAI;

class Tower: public game_space::Target {
public:
	enum SelectedWeapon {
		BULLET, MISSILE
	};
protected:
	Turret* _turret;

	PLYModel* _towerBase;

	float _baseWidth;

	TowerAI* _controllingAI;
	bool _isAIControlled;

	SelectedWeapon _selectedWeapon;
public:
	Tower(Scene &scene,bool isAIControlled);
	virtual ~Tower();
	void reset();

	/** Draw the object to the screen. */
	void draw() const;

	Point getMuzzlePosition() const;
	float getShootingPower() const;
	void setShootingPower(float power);

	float getElevation() const;
	void setElevation(float elevation);

	float getAzimuth() const;
	void setAzimuth(float azimuth);
	void update(float seconds);
	float getDirection() const;
	void setDirection(float angle);
	LookAt getLookAt() const;
	bool isAIControlled() const;
	TowerAI* getAI() const;
	void fireBullet();
	void fireMissile(Point target);
	void fireRobot();

	//only used for a human player
	SelectedWeapon getSelectedWeapon();
	void setSelectedWeapon(SelectedWeapon weapon);

};

} /* namespace game_space */
#endif /* TOWER_H_ */
