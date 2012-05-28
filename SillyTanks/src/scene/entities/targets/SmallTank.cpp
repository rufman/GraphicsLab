/*
 * smallTank.cpp
 * This class creates the smallest of all tanks.
 */

#include "SmallTank.hpp"

//common includes
#include "../../../common/Utils.hpp"
#include "../../../common/GLIncludes.hpp"

//scene includes
#include "../../../scene/Scene.hpp"
#include "../../../scene/Terrain.hpp"

#include "../../AI/TankAI.hpp"
#include "../collisiondetection/Projectile.hpp"
#include <algorithm>

namespace game_space {

SmallTank::SmallTank(Scene & scene, bool isAIControlled) :
		Tank(scene, isAIControlled) {
	_baseWidth = 1.0f;
	float turretHeight = 0.5f;
	float tankLength = 1.0f;
	float tankWidth = 1.0f;
	float tankHeight = 0.6f;

	//TODO: here something is weird with the axes of the coordinate system
	// maybe exchange x and y axis (width and length of the tank) for the boundingBox only..

	Point frontRightUnder = Point(0, 0, 0);
	Point frontLeftUnder = Point(0, -tankWidth, 0);
	Point frontRightUpper = Point(0, 0, tankHeight);
	Point frontLeftUpper = Point(0, -tankWidth, tankHeight);

	Point rearRightUnder = Point(tankLength, 0, 0);
	Point rearLeftUnder = Point(tankLength, -tankWidth, 0);
	Point rearRightUpper = Point(tankLength, 0, tankHeight);
	Point rearLeftUpper = Point(tankLength, -tankWidth, tankHeight);

	Point* pointArray = new Point[8];

	pointArray[0] = frontRightUnder;
	pointArray[1] = frontLeftUnder;
	pointArray[2] = frontRightUpper;
	pointArray[3] = frontLeftUpper;
	pointArray[4] = rearRightUnder;
	pointArray[5] = rearLeftUnder;
	pointArray[6] = rearRightUpper;
	pointArray[7] = rearLeftUpper;

	_boundingBox = new BoundingBox(pointArray);

	//create a new chassis / with texture size 400
	_chassis = new PLYModel(_scene);

	//get a new turret
	_turret = new Turret(_scene);

	int tankTextureNumber = rand() % 16;
	std::string tankTexture = "NO-TEXTURE-PATH";
	switch (tankTextureNumber) {
	case 0: {
		tankTexture = SMALLTANK_TEXTURE1;
		break;
	}
	case 1: {
		tankTexture = SMALLTANK_TEXTURE2;
		break;
	}
	case 2: {
		tankTexture = SMALLTANK_TEXTURE3;
		break;
	}
	case 3: {
		tankTexture = SMALLTANK_TEXTURE4;
		break;
	}
	case 4: {
		tankTexture = SMALLTANK_TEXTURE5;
		break;
	}
	case 5: {
		tankTexture = SMALLTANK_TEXTURE6;
		break;
	}
	case 6: {
		tankTexture = SMALLTANK_TEXTURE7;
		break;
	}
	case 7: {
		tankTexture = SMALLTANK_TEXTURE8;
		break;
	}
	case 8: {
		tankTexture = SMALLTANK_TEXTURE9;
		break;
	}
	case 9: {
		tankTexture = SMALLTANK_TEXTURE10;
		break;
	}
	case 10: {
		tankTexture = SMALLTANK_TEXTURE11;
		break;
	}
	case 11: {
		tankTexture = SMALLTANK_TEXTURE12;
		break;
	}
	case 12: {
		tankTexture = SMALLTANK_TEXTURE13;
		break;
	}
	case 13: {
		tankTexture = SMALLTANK_TEXTURE14;
		break;
	}
	case 14: {
		tankTexture = SMALLTANK_TEXTURE15;
		break;
	}
	case 15: {
		tankTexture = SMALLTANK_TEXTURE16;
		break;
	}
	default: {
		tankTexture = SMALLTANK_TEXTURE3;
		break;
	}
	}
	//_tankTexture = new TGATexture(tankTexture.c_str());
	_chassis->load(SMALLTANK_CHASSIS_MODEL, tankTexture.c_str());
	_turret->load(SMALLTANK_CHASSIS_MODEL, tankTexture.c_str());
	reset();

}

SmallTank::~SmallTank() {
}

void SmallTank::draw() const {
	//to draw the particle engine
	Tank::draw();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	glTranslatef(_position.x, _position.y, _position.z);

	Utils::applyGLRotation(Vector3D(0.0f, 0.0f, 1.0f), _scene.getTerrain().getNormal(_position));

	glScalef(_baseWidth * 5, _baseWidth * 5, _baseWidth * 5);

	//glRotation seems to be counter intuitive as we have to rotate in negative direction (check Utils::applyRotation)
	glRotatef(-getDirection(), 0, 0, 1);
	glTranslatef(0, -0.5, 0);

	_chassis->setRenderingParameters(_renderingParameters);
	_chassis->draw();

	//translate the turret to the right position
	glTranslatef(0, 0.5, 0.3);

	//scale down the turret
	glScalef(0.5, 0.5, 0.5);

	//rotate back the chassis turning
	glRotatef(getDirection(), 0, 0, 1);

	_turret->setRenderingParameters(_renderingParameters);
	Point tankPosition = getPosition();
	_turret->setPosition(Point(tankPosition.x, tankPosition.y, tankPosition.z));
	_turret->draw();
	glPopMatrix();
}

void SmallTank::castShadow(const Point &lightPostion) const {
	_chassis->castShadow(&lightPostion);
}

void SmallTank::setNeighbors() const {
	_chassis->setNeighbors();
}

bool Target::checkHit(Projectile* projectile_) {

	//you can not shoot yourself
	if(projectile_->_projectileOwner == this)
	{
		return false;
	}
	Point projectilePosition = projectile_->getPosition();

	//max and min to store the highest and smallest value temporary
	float max, min;

	//compute maximum height of bounding box (max z point)
	float frontMin = std::max(_boundingBox->frontLeftUpper.z, _boundingBox->frontRightUpper.z);
	float rearMin = std::max(_boundingBox->rearLeftUpper.z, _boundingBox->rearRightUpper.z);

	max = std::max(frontMin, rearMin) + _position.z;

	//compute minimum height of bounding box (min y point)
	frontMin = std::min(_boundingBox->frontLeftUnder.z, _boundingBox->frontRightUnder.z);
	rearMin = std::min(_boundingBox->rearLeftUnder.z, _boundingBox->rearRightUnder.z);

	min = std::min(frontMin, rearMin) + _position.z;

	//check if projectile is in the right height to hit the target
	if (projectilePosition.z > max || projectilePosition.z < min) {

		return false;
	}


	//##########################################################
	//the projectile has an appropriate height to hit the target

	//compute max x point
	frontMin = std::max(_boundingBox->frontLeftUpper.x, _boundingBox->rearLeftUpper.x);
	rearMin = std::max(_boundingBox->frontRightUpper.x, _boundingBox->rearRightUpper.x);

	max = std::max(frontMin, rearMin) + _position.x;

	//compute min x point
	frontMin = std::min(_boundingBox->frontLeftUpper.x, _boundingBox->rearLeftUpper.x);
	rearMin = std::min(_boundingBox->frontRightUpper.x, _boundingBox->rearRightUpper.x);

	min = std::min(frontMin, rearMin) + _position.x;

	//check if the projectile is between the right x coordinates to hit the target
	if (projectilePosition.x > max || projectilePosition.x < min) {
		return false;
	}

	//This part seems not to be used
//	//compute max y point
//	frontMin = std::max(_boundingBox->frontLeftUpper.y, _boundingBox->rearLeftUpper.y);
//	rearMin = std::max(_boundingBox->frontRightUpper.y, _boundingBox->rearRightUpper.y);
//
//	max = std::max(frontMin, rearMin) + _position.y;
//
//	//compute min z point
//
//	frontMin = std::min(_boundingBox->frontLeftUpper.y, _boundingBox->rearLeftUpper.y);
//	rearMin = std::min(_boundingBox->frontRightUpper.y, _boundingBox->rearRightUpper.y);
//
//	min = std::min(frontMin, rearMin) + _position.y;

	return true;

}

}
