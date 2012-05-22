/*
 * player.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */


// Class declaration include
#include "player.hpp"

// Includes
#include "gl_includes.hpp"
#include "defs.hpp"
#include "vector.hpp"
#include "map.hpp"
#include "scene.hpp"
#include "camera3D.hpp"
#include "utils.hpp"
#include <iostream>
#include <cmath>

GRAPHICSLAB_NAMESPACE_BEGIN

Player::Player(Scene& scene, Map* map) :
	_x(0.0),
	_y(0.0),
	_speed(0.0),
	_height(0.0),
	_direction(0.0),
	_rotationMode(0),
	_strafeDirection(0.0),
	_walking(false),
	_scene(scene),
	_map(map),
	_cameraZoom(0.0),
	_cameraElevation(0.0),
	_jumpHeight(0.0),
	_jumpSpeed(0.0),
	_walkingAnimationPosition(0.0),
    _numDisplayLists(5),
    _alternateJumpingAnimation(1.0),
    _displayLists(glGenLists(_numDisplayLists)) {
	_playerTexture = new TGATexture("data/textures/player.tga");
	_playerTexture->init();
}


Player::~Player() {
	delete _playerTexture;
}


void Player::init() {

	glNewList(_displayLists + 0, GL_COMPILE); // Body

	float bodyPointsX[]   = { 0.0,   0.25,  0.17, -0.17, -0.25,  0.0};
	float bodyPointsY[]   = { 0.60,  0.55, -0.10, -0.10,  0.55,  0.60};
	float bodyPointsZ1[]  = { 0.11,  0.12,  0.13,  0.13,  0.12,  0.11};
	float bodyPointsZ2[]  = {-0.05, -0.02, -0.09, -0.09, -0.02, -0.05};
	float bodyTexOffset[] = {  0.0, 100.0, 360.0, 496.0, 756.0, 856.0};
	vec3f bodyNormals[]   = {vec3f( 0.0,  1.0, 1.0),
							 vec3f( 1.0,  1.0, 1.0),
							 vec3f( 1.0, -1.0, 1.0),
							 vec3f(-1.0, -1.0, 1.0),
							 vec3f(-1.0,  1.0, 1.0),
							 vec3f( 0.0,  1.0, 1.0)};

	for(int i = 0; i < 6; i++) {
		Utils::normalize(bodyNormals[i]);
	}

	float textureOffsetX, textureOffsetY;

	textureOffsetX = 410.0;
	textureOffsetY = 250.0;

	glBegin(GL_TRIANGLE_FAN);
	for(int i = 0; i < 5; i++) {
		glTexCoord2f(textureOffsetX + (400.0 * bodyPointsX[i]), textureOffsetY - (400.0 * bodyPointsY[i]));
		glNormal3f(bodyNormals[i].x(), bodyNormals[i].y(), bodyNormals[i].z());
		glVertex3f(bodyPointsX[i], bodyPointsY[i], bodyPointsZ1[i]);
	}
	glEnd();

	textureOffsetX = 200.0;
	textureOffsetY = 250.0;

	glBegin(GL_TRIANGLE_FAN);
	for(int i = 0; i < 5; i++) {
		glTexCoord2f(textureOffsetX - (400.0 * bodyPointsX[i]), textureOffsetY - (400.0 * bodyPointsY[i]));
		glNormal3f(bodyNormals[i].x(), bodyNormals[i].y(), -bodyNormals[i].z());
		glVertex3f(bodyPointsX[i], bodyPointsY[i], bodyPointsZ2[i]);
	}
	glEnd();
	textureOffsetX = 10.0;
	textureOffsetY = 10.0;

	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < 6; i++) {
		glTexCoord2f(textureOffsetX, textureOffsetY + bodyTexOffset[i]);
		glNormal3f(bodyNormals[i].x(), bodyNormals[i].y(), bodyNormals[i].z());
		glVertex3f(bodyPointsX[i], bodyPointsY[i], bodyPointsZ1[i]);

		glTexCoord2f(textureOffsetX + 80.0, textureOffsetY + bodyTexOffset[i]);
		glNormal3f(bodyNormals[i].x(), bodyNormals[i].y(), -bodyNormals[i].z());
		glVertex3f(bodyPointsX[i], bodyPointsY[i], bodyPointsZ2[i]);
	}
	glEnd();

	glEndList();




	glNewList(_displayLists + 1, GL_COMPILE); // Head

	glRotatef(-90, 0.0, 1.0, 0.0);
	Utils::drawTexturedCylinder(0.06, 0.06, 0.18, 16, 100.0, 480.0, 144.0, 72.0, true);
	glRotatef(-90, 0.0, -1.0, 0.0);

	glTranslatef(0.0, 0.18, -0.03);
	glScalef(0.20, 0.28, 0.24);

	glRotatef(-90, 0.0, 1.0, 0.0);
	Utils::drawTexturedSphere(0.5, 24, 12, 100.0, 300.0, 240.0, 168.0);
	glRotatef(-90, 0.0, -1.0, 0.0);

	glScalef(1.0 / 0.20, 1.0 / 0.28, 1.0 / 0.24);
	glTranslatef(0.0, -0.18, 0.03);

	glEndList();




	glNewList(_displayLists + 2, GL_COMPILE); // Arm

	glRotatef(180, 0.0, 1.0, 0.0);
	Utils::drawTexturedCylinder(0.06, 0.05, 0.37, 16, 100.0, 728.0, 120.0, -148.0, true);
	glRotatef(180, 0.0, -1.0, 0.0);

	glTranslatef(0.0, 0.33, 0.0);
	glRotatef(45.0, 1.0, 0.0, 0.0);

	glRotatef(180, 0.0, 1.0, 0.0);
	Utils::drawTexturedCylinder(0.05, 0.04, 0.37, 16, 100.0, 876.0, 120.0, -148.0, true);
	glRotatef(180, 0.0, -1.0, 0.0);

	glRotatef(45.0, -1.0, 0.0, 0.0);
	glTranslatef(0.0, -0.33, 0.0);

	glEndList();




	glNewList(_displayLists + 3, GL_COMPILE); // Upper leg

	glRotatef(180.0, 1.0, 0.0, 0.0);

	glRotatef(180, 0.0, 1.0, 0.0);
	Utils::drawTexturedCylinder(0.08, 0.07, 0.48, 16, 350.0, 480.0, 120.0, -180.0, true);
	glRotatef(180, 0.0, -1.0, 0.0);

	glRotatef(180.0, -1.0, 0.0, 0.0);

	glEndList();




	glNewList(_displayLists + 4, GL_COMPILE); // Lower leg

	glRotatef(180.0, 1.0, 0.0, 0.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	Utils::drawTexturedCylinder(0.07, 0.05, 0.45, 16, 350.0, 684.0, 120.0, -192.0, true);
	glRotatef(180, 0.0, -1.0, 0.0);
	glRotatef(180.0, -1.0, 0.0, 0.0);


	glTranslatef(0.00, -0.45, 0.0);
	glRotatef(-10.0, 0.0, 1.0, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.00, -0.05, 0.0);

	Utils::drawTexturedHalfCylinder(0.05, 0.05, 0.28, 8, 350.0, 784.0, 120.0, -112.0, true);

	glTranslatef(0.00, 0.05, 0.0);
	glRotatef(-90.0, -1.0, 0.0, 0.0);
	glRotatef(-10.0, 0.0, -1.0, 0.0);
	glTranslatef(0.00, 0.45, 0.0);

	glEndList();
}


vec3f Player::getPosition() {
	return vec3f(_x, _height, _y);
}

float Player::getDirection() {
	return _direction;
}

vec3f Player::getCameraPosition() {
	return vec3f(_cameraPositionX, _cameraPositionH, _cameraPositionY);
}

float Player::getCameraAngle() {
	return _direction;
}

float Player::getCameraElevation() {
	return _cameraElevation;
}

float Player::getCameraZoom() {
	return _cameraZoom;
}

LookAt Player::getCameraLookAt() {
    Point from(_cameraPositionX, _cameraPositionH, _cameraPositionY);
    Point to(_x + 					( sin(Utils::toRadian(_direction)) * cos(Utils::toRadian(_cameraElevation))),
    		 _height + _jumpHeight + EYE_HEIGHT + (-sin(Utils::toRadian(_cameraElevation))),
    		 _y + 					(-cos(Utils::toRadian(_direction)) * cos(Utils::toRadian(_cameraElevation))));
    return LookAt(from, to, vec3f(0.0, 1.0, 0.0));
}

void Player::setCameraElevationZoom(float elevation, float zoom) {
	_cameraElevation = elevation;
	_cameraZoom = zoom;
	updateCamera();
}

void Player::place(float x, float y, float direction) {
	_x = x;
	_y = y;
	_direction = direction;
	_height = _map->getSurfaceHeight(_x, _y);
	_jumpHeight = 0.0;
	updateCamera();
}


void Player::update(float duration) {

	float speedGoal = 0.0;
	_rotationMode = 0;
	bool move = false;

	bool topKeyHeld    = (InputManager::specialKeyHeld(GLUT_KEY_UP   ) || InputManager::keyHeld('w')) && !InputManager::keyHeld('s') && !InputManager::specialKeyHeld(GLUT_KEY_DOWN );
	bool bottomKeyHeld = (InputManager::specialKeyHeld(GLUT_KEY_DOWN ) || InputManager::keyHeld('s')) && !InputManager::keyHeld('w') && !InputManager::specialKeyHeld(GLUT_KEY_UP   );
	bool leftKeyHeld   = (InputManager::specialKeyHeld(GLUT_KEY_LEFT ) || InputManager::keyHeld('a')) && !InputManager::keyHeld('d') && !InputManager::specialKeyHeld(GLUT_KEY_RIGHT);
	bool rightKeyHeld  = (InputManager::specialKeyHeld(GLUT_KEY_RIGHT) || InputManager::keyHeld('d')) && !InputManager::keyHeld('a') && !InputManager::specialKeyHeld(GLUT_KEY_LEFT );

	if(InputManager::keyUp('x')) {
		_walking = !_walking;
	}

	if(topKeyHeld) {
		move = true;
		_strafeDirection = 0.0;

		if(leftKeyHeld) {
			_strafeDirection = -45.0;
		}
		if(rightKeyHeld) {
			_strafeDirection = 45.0;
		}

	} else if(bottomKeyHeld) {
		move = true;
		_strafeDirection = 180.0;

		if(leftKeyHeld) {
			_strafeDirection = -135.0;
		}
		if(rightKeyHeld) {
			_strafeDirection = 135.0;
		}

	} else {

		if(leftKeyHeld) {
			move = true;
			_strafeDirection = -90.0;
		}
		if(rightKeyHeld) {
			move = true;
			_strafeDirection = 90.0;
		}

	}

	if(InputManager::keyHeld('q') && !InputManager::keyHeld('e')) {
		_rotationMode = -1;
	}

	if(InputManager::keyHeld('e') && !InputManager::keyHeld('q')) {
		_rotationMode = 1;
	}

	if(move) {
		if(_walking) {
			speedGoal = WALKING_SPEED;
		} else {
			speedGoal = RUNNING_SPEED;
		}
	}

	if(_speed < speedGoal) {
		_speed += ACCELERATION * duration;
		if(_speed > speedGoal) {
			_speed = speedGoal;
		}
	} else if(_speed > speedGoal) {
		_speed -= ACCELERATION * duration;
		if(_speed < speedGoal) {
			_speed = speedGoal;
		}
	}

	float rotationSpeed = ROTATION_SPEED * (float)_rotationMode;

	if(InputManager::mouseHeld(2)) {
		_direction += 360.0 * ((float)InputManager::mouseDeltaX() / (float)glutGet(GLUT_WINDOW_WIDTH));
	}

	_direction += rotationSpeed * duration;

	if(_direction > 180) {
		_direction -= 360;
	}
	if(_direction <= -180) {
		_direction += 360;
	}

	if(_map->getObstructingObjectF(_x, _y) == 3) {
		_map->setObstructingObjectF(_x, _y, 0);
	}

	_xNext = _x + duration * _speed *  sin(Utils::toRadian(_direction + _strafeDirection));
	_yNext = _y +duration * _speed * -cos(Utils::toRadian(_direction + _strafeDirection));

	float oldHeight = _height;
	_heightNext = _map->getSurfaceHeight(_xNext, _yNext);
	
	if(InputManager::keyHeld(' ') && _jumpHeight == 0.0) {
		_jumpSpeed = INITIAL_JUMP_SPEED;
		_alternateJumpingAnimation += 1.0;
		if(_alternateJumpingAnimation >= 2.0) {
			_alternateJumpingAnimation = 0.0;
		}
	}
	
	float waterLevel = (float)Map::LEVEL_OF_WATER * Map::VERTICAL_SCALE;
	int obstruction = _map->getObstructingObjectF(_xNext, _yNext);
	float inclination = (_heightNext - _height) / (sqrt(pow(_xNext - _x, 2) + pow(_yNext - _y, 2)));
	bool isJumping = _jumpHeight != 0.0 || _jumpSpeed == INITIAL_JUMP_SPEED;

	//Update Position if possible
	if(_heightNext >= waterLevel
	   && (obstruction == 0 || (obstruction == 2 && isJumping))
	   && inclination < 1.0) {

		_x = _xNext;
		_y = _yNext;
		_height = _heightNext;
		
	} else {
		_speed = 0.0;
	}

	if(_map->getObstructingObjectF(_x, _y) == 0) {
		_map->setObstructingObjectF(_x, _y, 3);
	}

	if(_jumpHeight != 0.0 || _jumpSpeed > 0.0) {
		_jumpSpeed += GRAVITY_ACCELERATION * duration;
		_jumpHeight += _jumpSpeed * duration;
		_jumpHeight -= _height - oldHeight;
	}
	if(_jumpHeight < 0.0) {
		_jumpHeight = 0.0;
	}
	
	if(_walking) {
		_walkingAnimationPosition += duration * 2.0;
	} else {
		_walkingAnimationPosition += duration * 3.0;
	}
	if(_walkingAnimationPosition > 2.0) {
		_walkingAnimationPosition -= 2.0;
	}
	if(_speed == 0.0) {
		_walkingAnimationPosition = 0.0;
	}
	if(_jumpHeight > 0.0 && _speed > 0.0) {
		float animationHeight = _jumpHeight;
		if(animationHeight > 0.5) {
			animationHeight = 0.5;
		}
		if(_jumpSpeed > 0) {
			_walkingAnimationPosition = animationHeight + _alternateJumpingAnimation;
		} else {
			_walkingAnimationPosition = (1 - animationHeight) + _alternateJumpingAnimation;
		}

	}

	updateCamera();
}



void Player::updateCamera() {
	if(InputManager::mouseHeld(2)) {
		_cameraElevation += 360.0 * ((float)InputManager::mouseDeltaY() / (float)glutGet(GLUT_WINDOW_HEIGHT));
		InputManager::stickyMouse();
	}
	if(_cameraElevation > 89.0) {
		_cameraElevation = 89.0;
	}
	if(_cameraElevation < -60.0) {
		_cameraElevation = -60.0;
	}

	if(InputManager::mouseHeld(1)) {
		_cameraZoom += 8.0 * ((float)InputManager::mouseDeltaY() / (float)glutGet(GLUT_WINDOW_HEIGHT));
		InputManager::stickyMouse();
	}
	if(_cameraZoom > 8.0) {
		_cameraZoom = 8.0;
	}
	if(_cameraZoom < 0.0) {
		_cameraZoom = 0.0;
	}

	_cameraPositionX = _x + (_cameraZoom * -sin(Utils::toRadian(_direction)) * cos(Utils::toRadian(_cameraElevation)));
	_cameraPositionH = _height + _jumpHeight + EYE_HEIGHT + (_cameraZoom * sin(Utils::toRadian(_cameraElevation)));
	_cameraPositionY = _y + (_cameraZoom *  cos(Utils::toRadian(_direction)) * cos(Utils::toRadian(_cameraElevation)));

	float surfaceHeightAtCamera = _map->getSurfaceHeight(_cameraPositionX, _cameraPositionY);
	if(_cameraPositionH < surfaceHeightAtCamera + MIN_DISTANCE_CAMERA_SURFACE) {
		_cameraPositionH = surfaceHeightAtCamera + MIN_DISTANCE_CAMERA_SURFACE;
	}
}


void Player::draw() {

	if(MenuManager::isInIntroMenuMode()) {
		return;
	}
	if(_cameraZoom < 0.4 && !ReflectionTexture::isDrawing() && !ShadowTexture::isDrawing()) {
		return;
	}

	float speedMovementRatio;
	if(_walking) {
		speedMovementRatio = _speed / (WALKING_SPEED * 2);
	} else {
		speedMovementRatio = _speed / RUNNING_SPEED;
	}


	glPushMatrix();

	_map->applyReflectionTransformation();
	glTranslatef(_x, _height, _y);
	_map->applyShadowTransformation();
	glTranslatef(0.0, _jumpHeight, 0.0);

	glRotatef(_direction + _strafeDirection, 0.0, -1.0, 0.0);

	if(ShadowTexture::isDrawing()) {
		glColor3f(0.0, 0.0, 0.0);
	} else {
		glColor3f(1.0, 1.0, 1.0);
	}

	_playerTexture->setActive(true);


	float verticalRunningMovement = 0.02 - (0.02 * cos(_walkingAnimationPosition * 3.142 * 2) * speedMovementRatio);
	float bodyTurn = 10.0 * sin(_walkingAnimationPosition * 3.142) * speedMovementRatio;

	glTranslatef(0.0, 0.90 + verticalRunningMovement, 0.0);
	glRotatef(-30 * speedMovementRatio, 1.0, 0.0, 0.0);


	glTranslatef(0.0, 0.58, 0.03);

	glRotatef(-20 * speedMovementRatio, -1.0, 0.0, 0.0);
	glCallList(_displayLists + 1); // Head
	glRotatef(-20 * speedMovementRatio, 1.0, 0.0, 0.0);

	glTranslatef(0.0, -0.58, -0.03);



	glRotatef(bodyTurn, 0.0, 1.0, 0.0);


	glCallList(_displayLists + 0); // Body


	float armRotation = 60.0 * sin(_walkingAnimationPosition * 3.142) * speedMovementRatio;

	glTranslatef(0.21, 0.52, 0.05);
	glRotatef(170.0 + (10 * speedMovementRatio), 1.0, 0.0, 0.0);
	glRotatef(-10.0, 0.0, 0.0, 1.0);
	glRotatef(-20.0, 0.0, 1.0, 0.0);

	glRotatef(armRotation, 1.0, 0.0, 0.0);
	glCallList(_displayLists + 2); // Arm
	glRotatef(armRotation, -1.0, 0.0, 0.0);

	glRotatef(-20.0, 0.0, -1.0, 0.0);
	glRotatef(-10.0, 0.0, 0.0, -1.0);
	glRotatef(170.0 + (10 * speedMovementRatio), -1.0, 0.0, 0.0);
	glTranslatef(-0.21, -0.52, -0.05);


	glScalef(-1.0, 1.0, 1.0);
	glTranslatef(0.21, 0.52, 0.05);
	glRotatef(170.0 + (10 * speedMovementRatio), 1.0, 0.0, 0.0);
	glRotatef(-10.0, 0.0, 0.0, 1.0);
	glRotatef(-20.0, 0.0, 1.0, 0.0);

	glRotatef(armRotation, -1.0, 0.0, 0.0);
	glCallList(_displayLists + 2); // Arm
	glRotatef(armRotation, 1.0, 0.0, 0.0);

	glRotatef(-20.0, 0.0, -1.0, 0.0);
	glRotatef(-10.0, 0.0, 0.0, -1.0);
	glRotatef(170.0 + (10 * speedMovementRatio), -1.0, 0.0, 0.0);
	glTranslatef(-0.21, -0.52, -0.05);
	glScalef(-1.0, 1.0, 1.0);


	glRotatef(bodyTurn, 0.0, -1.0, 0.0);
	glRotatef(-30 * speedMovementRatio, -1.0, 0.0, 0.0);



	float upperLegRotation = 0.0;
	if(_walkingAnimationPosition < 0.5) {
		upperLegRotation = (90 * _walkingAnimationPosition) * speedMovementRatio;
	} else if(_walkingAnimationPosition < 1.5) {
		upperLegRotation = (45.0 - 90 * (_walkingAnimationPosition - 0.5)) * speedMovementRatio;
	} else {
		upperLegRotation = (90 * (_walkingAnimationPosition - 2.0)) * speedMovementRatio;
	}

	float rightLowerLegRotation = 0.0;
	float leftLowerLegRotation = 0.0;

	if(_walkingAnimationPosition >= 0.0 && _walkingAnimationPosition < 0.5) {
		leftLowerLegRotation = (120.0 * 2.0 * (0.5 - _walkingAnimationPosition)) * speedMovementRatio;
	} else if(_walkingAnimationPosition > 0.5 && _walkingAnimationPosition < 1.0) {
		rightLowerLegRotation = (120.0 * 2.0 * (_walkingAnimationPosition - 0.5)) * speedMovementRatio;
	} else if(_walkingAnimationPosition >= 1.0 && _walkingAnimationPosition < 1.5) {
		rightLowerLegRotation = (120.0 * 2.0 * (1.5 - _walkingAnimationPosition)) * speedMovementRatio;
	} else if(_walkingAnimationPosition > 1.5 && _walkingAnimationPosition <= 2.0) {
		leftLowerLegRotation = (120.0 * 2.0 * (_walkingAnimationPosition - 1.5)) * speedMovementRatio;
	}

	glTranslatef(0.08, 0.0, 0.0);

	glRotatef(upperLegRotation, -1.0, 0.0, 0.0);
	glCallList(_displayLists + 3); // Upper leg

	glTranslatef(0.00, -0.45, 0.0);

	glRotatef(rightLowerLegRotation, -1.0, 0.0, 0.0);
	glCallList(_displayLists + 4); // Lower leg
	glRotatef(rightLowerLegRotation, 1.0, 0.0, 0.0);

	glTranslatef(0.00, 0.45, 0.0);
	glRotatef(upperLegRotation, 1.0, 0.0, 0.0);

	glTranslatef(-0.08, -0.0, 0.0);




	glScalef(-1.0, 1.0, 1.0);
	glTranslatef(0.08, 0.0, 0.0);

	glRotatef(upperLegRotation, 1.0, 0.0, 0.0);
	glCallList(_displayLists + 3); //Upper leg

	glTranslatef(0.00, -0.45, 0.0);
	glRotatef(leftLowerLegRotation, -1.0, 0.0, 0.0);
	glCallList(_displayLists + 4); // Lower leg
	glRotatef(leftLowerLegRotation, 1.0, 0.0, 0.0);
	glTranslatef(0.00, 0.45, 0.0);

	glRotatef(upperLegRotation, -1.0, 0.0, 0.0);

	glTranslatef(-0.08, 0.0, 0.0);
	glScalef(-1.0, 1.0, 1.0);



	glTranslatef(0.0, -(0.90 + verticalRunningMovement), 0.0);

	_playerTexture->setActive(false);

	glRotatef(-(_direction + _strafeDirection), 0.0, -1.0, 0.0);

	_map->removeShadowTransformation();
	_map->removeReflectionTransformation();

	glPopMatrix();
}

GRAPHICSLAB_NAMESPACE_END


