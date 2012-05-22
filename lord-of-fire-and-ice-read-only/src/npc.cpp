/*
 * npc.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */


// Class declaration include
#include "npc.hpp"

// Includes
#include "gl_includes.hpp"
#include "drawable.hpp"
#include "defs.hpp"
#include "vector.hpp"
#include "map.hpp"
#include "scene.hpp"
#include "utils.hpp"
#include <cmath>
#include <cstdlib>

GRAPHICSLAB_NAMESPACE_BEGIN

DrawableNPC::DrawableNPC(Scene& scene, NPC* npc) :
	Drawable(scene, 0.0, 0.0, 0.0),
	_npc(npc) {
}

void DrawableNPC::draw(int quality) {
	_npc->draw(quality);
}

TGATexture* NPC::_npcTexture;
uint NPC::_numDisplayLists;
GLuint NPC::_displayLists;

NPC::NPC(Scene& scene, Map* map, Player* player) :
	Drawable(scene, 0.0, 0.0, 0.0),
	_drawable(NULL),
	_id(-1),
	_spawnX(0.0),
	_spawnY(0.0),
	_spawnDirection(0.0),
	_oriX(0.0),
	_oriY(0.0),
	_nxtX(0.0),
	_nxtY(0.0),
	_dstX(0.0),
	_dstY(0.0),
	_speed(0.0),
	_direction(0.0),
	_walking(false),
	_map(map),
	_player(player),
	_sectorID(-1),
	_restlessnessRating(0.0),
	_restlessness(0.0),
	_walkingAnimationPosition(0.0) {
	_path = PointList();
	_tShirtColor = Color3f(1.0, 1.0, 1.0);
}


NPC::~NPC() {
}


void NPC::init() {

	_numDisplayLists = 5;
	_displayLists = glGenLists(_numDisplayLists);

	_npcTexture = new TGATexture("data/textures/npc.tga");
	_npcTexture->init();

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


vec3f NPC::getPosition() {
	return vec3f(_x, _altitude, _y);
}

float NPC::getDirection() {
	return _direction;
}


void NPC::place(float x, float y, float direction) {
	if(_map->getObstructingObjectF(_x, _y) == _id) {
		_map->setObstructingObjectF(_x, _y, 0);
	}
	_x = x;
	_y = y;
	_oriX = x;
	_oriY = y;
	_nxtX = x;
	_nxtY = y;
	_dstX = x;
	_dstY = y;
	_direction = direction;
	_altitude = _map->getSurfaceHeight(_x, _y);
	_restlessnessRating = 0.0;
	_path = PointList();
	updateSectorID();
	_map->setObstructingObjectF(_x, _y, _id);
}


void NPC::update(float duration) {
	vec3f playerPosition = _player->getPosition();
	float distanceToPlayer = Utils::distance(_x, _y, playerPosition.x(), playerPosition.z());

	float maxSpeed;
	if(_walking) {
		maxSpeed = WALKING_SPEED;
	} else {
		maxSpeed = RUNNING_SPEED;
	}

	if(_restlessness > 0.0 && _dstX == _x && _dstY == _y) {
		_restlessnessRating += duration;
	}

	if(_restlessnessRating > _restlessness && distanceToPlayer > 4.0) {
		_restlessnessRating = 0.0;

		float randomDirection = (float)(rand() % 360);
		float randomDistance = ((float)(rand() % 101) / 101.0) * 30.0;
		_dstX = round(_spawnX + (sin(Utils::toRadian(randomDirection)) * randomDistance));
		_dstY = round(_spawnY + (cos(Utils::toRadian(randomDirection)) * randomDistance));
		while(_map->getObstructingObjectF(_dstX, _dstY) != 0) {
			randomDirection = (float)(rand() % 360);
			randomDistance = ((float)(rand() % 101) / 101.0) * 30.0;
			_dstX = round(_spawnX + (sin(Utils::toRadian(randomDirection)) * randomDistance));
			_dstY = round(_spawnY + (cos(Utils::toRadian(randomDirection)) * randomDistance));
		}
		moveTo(_dstX, _dstY);
	}

	if(_x == _nxtX && _y == _nxtY) {
		passNode();
	}
	float deltaToNxtX = _nxtX - _x;
	float deltaToNxtY = _nxtY - _y;
	float distanceToNxt = Utils::distance(_x, _y, _nxtX, _nxtY);
	float distanceToDst = Utils::distance(_x, _y, _dstX, _dstY);
	float speedRatioX = deltaToNxtX / distanceToNxt;
	float speedRatioY = deltaToNxtY / distanceToNxt;

	float breakingDistance = ((_speed * _speed) / (2.0 * ACCELERATION));
	if(breakingDistance > distanceToDst) {
    	_speed = sqrt(2.0 * distanceToDst * ACCELERATION);
	} else {
    	if(_speed < maxSpeed) {
    		_speed += ACCELERATION * duration;
    	}
    	if(_speed > maxSpeed) {
    		_speed = maxSpeed;
    	}
	}

	float stepX = _speed * speedRatioX * duration;
	float stepY = _speed * speedRatioY * duration;
	if(((stepX * stepX >= deltaToNxtX * deltaToNxtX) && (stepY * stepY >= deltaToNxtY * deltaToNxtY))
	   || _speed <= 0.0
	   || (deltaToNxtX * deltaToNxtX < 0.00001 && deltaToNxtY * deltaToNxtY < 0.00001)) {
		if(_path.empty()) {
			_speed = 0.0;
		}
		_x = _nxtX;
		_y = _nxtY;
		passNode();
	} else {
		_x += stepX;
		_y += stepY;
	}

	if(_x != _nxtX || _y != _nxtY ) {
		_direction = Utils::getAzimuth(_x, _y, _nxtX, _nxtY);
	}

	_altitude = _map->getSurfaceHeight(_x, _y);
	
	updateSectorID();

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
}


void NPC::moveTo(float x, float y) {
	PathFinding pathFinder = PathFinding(_map, _id, _x, _y, x, y, 0.5);
	_path = pathFinder.find();
	if(_path.empty()) {
		_dstX = _x;
		_dstY = _y;
	}
	passNode();
}


void NPC::passNode() {
	if(!_path.empty()) {
		_map->setObstructingObjectF(_oriX, _oriY, 0);
		_map->setObstructingObjectF(_nxtX, _nxtY, 0);
    	_oriX = _nxtX;
    	_oriY = _nxtY;
    	Point nxt = _path.back();
    	_path.pop_back();
    	_nxtX = nxt.x;
    	_nxtY = nxt.z;
    	if(_map->getObstructingObjectF(_nxtX, _nxtY) != 0) {
    		moveTo(_dstX, _dstY);
    		_map->setObstructingObjectF(_nxtX, _nxtY, _id);
    	}
    	_map->setObstructingObjectF(_oriX, _oriY, _id);
	}
}


void NPC::draw(int quality) {
	float speedMovementRatio;
	if(_walking) {
		speedMovementRatio = _speed / (WALKING_SPEED * 2);
	} else {
		speedMovementRatio = _speed / RUNNING_SPEED;
	}

	glPushMatrix();

	_map->applyReflectionTransformation();
	glTranslatef(_x, _altitude, _y);
	_map->applyShadowTransformation();

	glRotatef(_direction, 0.0, -1.0, 0.0);

	if(ShadowTexture::isDrawing()) {
		glColor3f(0.0, 0.0, 0.0);
	} else {
		glColor3f(1.0, 1.0, 1.0);
	}

	_npcTexture->setActive(true);

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

	glColor3f(_tShirtColor.r, _tShirtColor.g, _tShirtColor.b);
	glCallList(_displayLists + 0); // Body
	glColor3f(1.0, 1.0, 1.0);

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

	_npcTexture->setActive(false);

	glRotatef(-_direction, 0.0, -1.0, 0.0);

	_map->removeShadowTransformation();
	_map->removeReflectionTransformation();

	glPopMatrix();
}

void NPC::updateSectorID() {
	int newSectorID = _map->getSectorID(_x, _y);
	if(newSectorID != _sectorID) {
		Sector* oldSector = _map->getSectorByID(_sectorID);
		if(oldSector != NULL) {
			oldSector->removeObject(_drawable);
			delete _drawable;
		}
		_drawable = new DrawableNPC(_scene, this);
		_map->getSectorByID(newSectorID)->addObject(_drawable);
		_sectorID = newSectorID;
	}
}

void NPC::remove() {
	Sector* sector = _map->getSectorByID(_sectorID);
	if(sector != NULL) {
		sector->removeObject(_drawable);
		delete _drawable;
	}
	_sectorID = -1;
}

void NPC::setSpawnPosition(float x, float y) {
	_spawnX = x;
	_spawnY = y;
}

void NPC::setSpawnDirection(float direction) {
	_spawnDirection = direction;
}

void NPC::spawn() {
	place(_spawnX, _spawnY, _spawnDirection);
}

void NPC::assignID(int id) {
	_id = id;
}

void NPC::setRestlessness(float restlessness) {
	_restlessness = restlessness;
}

void NPC::setLocomotionMode(bool isWalking) {
	_walking = isWalking;
}

void NPC::setTShirtColor(float red, float green, float blue) {
	_tShirtColor = Color3f(red, green, blue);
}

GRAPHICSLAB_NAMESPACE_END


