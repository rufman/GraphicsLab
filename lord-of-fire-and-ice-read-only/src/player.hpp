/*
 * player.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef PLAYER_HPP_
#define PLAYER_HPP_


// Includes
#include "defs.hpp"
#include "map.hpp"
#include "scene.hpp"
#include "camera3D.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

// Forward declarations
class Map;


class Player {

public:

	/** Constructor */
	Player(Scene& scene, Map* map);

	/** Deconstructor */
	~Player();

	/** Initialise Player */
	void init();

	/** Get position of player */
	vec3f getPosition();

	/** Get direction of player */
	float getDirection();

	/** Get camera position */
	vec3f getCameraPosition();

	/** Get camera angle */
	float getCameraAngle();

	/** Get camera elevation */
	float getCameraElevation();

	/** Get camera zoom (these comments are so helpful) */
	float getCameraZoom();

	/** Set camera elevation and zoom */
	void setCameraElevationZoom(float elevation, float zoom);

	/** Get LookAt of player for camera */
	LookAt getCameraLookAt();

	/** Place player at position */
	void place(float x, float y, float direction);

	/** Update player */
	void update(float duration);
	void move(float duration_); //handles the moving part of  update
	

	/** Update camera */
	void updateCamera();

	/** Draw player Character */
	void draw();

	/** Keyboard input */
	void onKey(unsigned char key);
	void onKeyUp(unsigned char key);

private:
	static const float WALKING_SPEED = 1.5;
	static const float RUNNING_SPEED = 4.5;
	static const float ROTATION_SPEED = 45.0;
	static const float ACCELERATION = 20.0;
	static const float EYE_HEIGHT = 1.7;
	static const float MIN_DISTANCE_CAMERA_SURFACE = 0.5;
	static const float INITIAL_JUMP_SPEED = 3.5;
	static const float GRAVITY_ACCELERATION = -9.81;

	TGATexture* _playerTexture;

	Scene& _scene;
	Map* _map;

	float _x;
	float _y;
	float _height;
	float _xNext;
	float _yNext;
	float _heightNext;

	float _speed;
	float _direction;
	int _rotationMode;
	float _strafeDirection;
	bool _walking;
	float _walkingAnimationPosition;
	float _alternateJumpingAnimation;

	float _jumpHeight;
	float _jumpSpeed;

	float _cameraPositionX;
	float _cameraPositionY;
	float _cameraPositionH;
	float _cameraZoom;
	float _cameraElevation;

	uint _numDisplayLists;
	GLuint _displayLists;
};


GRAPHICSLAB_NAMESPACE_END

#endif /* PLAYER_HPP_ */
