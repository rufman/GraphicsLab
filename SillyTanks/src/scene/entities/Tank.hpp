/*
 * tank.hpp
* See tank.cpp
 */

#ifndef TANK_HPP_
#define TANK_HPP_

// common includes
#include "../../common/Drawable.hpp"
#include "../../common/Types.hpp"
#include "../../common/PLYModel.hpp"
#include "../../common/Camera3D.hpp"

//entities include
#include "Turret.hpp"



GAME_NAMESPACE_BEGIN


class Tank: public Drawable {
public:
	Tank(Scene &scene);
	virtual ~Tank();
	void reset();

    /** Draw the object to the screen. */
    void draw() const;

	Point getMuzzlePosition() const;
    float getShootingPower() const;
    void setShootingPower(float power);

    float getElevation() const;
    void setElevation( float elevation );

    float getAzimuth() const;
    void setAzimuth( float azimuth );
    void move(float speed);
    void update(float seconds);
    float getDirection() const;
    void setDirection(float angle);
    LookAt getLookAt() const;
    Point getPosition() const;

protected:
	Turret* _turret;

	PLYModel* _chassis;

	/** Ship position */
	Point _position;

	/** Ship velocity */
	Vector3D _velocity;

	float _speed;

	float _direction;

	float _baseWidth;

};

GAME_NAMESPACE_END
#endif
