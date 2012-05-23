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

#include <algorithm>

namespace game_space {

SmallTank::SmallTank(Scene & scene,bool isAIControlled):Tank(scene,isAIControlled) {
	_baseWidth = 1.0f;
	float turretHeight = 0.5f;
	float tankLength = 1.0f;
	float tankWidth = 1.0f;
	float tankHeight = 0.6f;

	Point frontRightUnder = Point(0,0,0);
	Point frontLeftUnder = Point(-tankWidth,0,0);
	Point frontRightUpper = Point(0,0,tankHeight);
	Point frontLeftUpper = Point(-tankWidth,0,tankHeight);

	Point rearRightUnder = Point(0,tankLength,0);
	Point rearLeftUnder = Point(-tankWidth, tankLength,0);
	Point rearRightUpper = Point(0, tankLength, tankHeight);
	Point rearLeftUpper = Point(-tankWidth, tankLength, tankHeight);

	Point* pointArray = new Point[8];

	pointArray[0] =  frontRightUnder;
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

	int tankTextureNumber = rand()%16;
	std::string tankTexture = "NO-TEXTURE-PATH";
	switch(tankTextureNumber)
	{
	case 0:
	{
		tankTexture = SMALLTANK_TEXTURE1;
		break;
	}
	case 1:
	{
		tankTexture = SMALLTANK_TEXTURE2;
		break;
	}
	case 2:
	{
		tankTexture = SMALLTANK_TEXTURE3;
		break;
	}
	case 3:
	{
		tankTexture = SMALLTANK_TEXTURE4;
		break;
	}
	case 4:
	{
		tankTexture = SMALLTANK_TEXTURE5;
		break;
	}
	case 5:
	{
		tankTexture = SMALLTANK_TEXTURE6;
		break;
	}
	case 6:
	{
		tankTexture = SMALLTANK_TEXTURE7;
		break;
	}
	case 7:
	{
		tankTexture = SMALLTANK_TEXTURE8;
		break;
	}
	case 8:
	{
		tankTexture = SMALLTANK_TEXTURE9;
		break;
	}
	case 9:
	{
		tankTexture = SMALLTANK_TEXTURE10;
		break;
	}
	case 10:
	{
		tankTexture = SMALLTANK_TEXTURE11;
		break;
	}
	case 11:
	{
		tankTexture = SMALLTANK_TEXTURE12;
		break;
	}
	case 12:
	{
		tankTexture = SMALLTANK_TEXTURE13;
		break;
	}
	case 13:
	{
		tankTexture = SMALLTANK_TEXTURE14;
		break;
	}
	case 14:
	{
		tankTexture = SMALLTANK_TEXTURE15;
		break;
	}
	case 15:
	{
		tankTexture = SMALLTANK_TEXTURE16;
		break;
	}
	default:
	{
		tankTexture = SMALLTANK_TEXTURE3;
		break;
	}
	}
	//_tankTexture = new TGATexture(tankTexture.c_str());
	_chassis->load(SMALLTANK_CHASSIS_MODEL,tankTexture.c_str());
	_turret->load(SMALLTANK_CHASSIS_MODEL,tankTexture.c_str());
	reset();

}

SmallTank::~SmallTank() {}

void SmallTank::draw() const
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	glTranslatef(_position.x,_position.y,_position.z);

	Utils::applyGLRotation(Vector3D(0.0f,1.0f,0.0f),_scene.getTerrain().getNormal(_position));

	glScalef(_baseWidth*5,_baseWidth*5,_baseWidth*5);
	glRotatef(-90,1,0,0);

	//glRotation seems to be counter intuitive as we have to rotate in negative direction (check Utils::applyRotation)
	glRotatef(-getDirection(),0,0,1);
	glTranslatef(0,-0.5,0);

	_chassis->setRenderingParameters(_renderingParameters);
	//_tankTexture->setActive(true);
	_chassis->draw();
	//_tankTexture->setActive(false);
	glPopMatrix();

	glPushMatrix();
	_turret->setRenderingParameters(_renderingParameters);
	Point tankPosition = getPosition();
	_turret->setPosition(Point(tankPosition.x,tankPosition.y,tankPosition.z));
	//_tankTexture->setActive(true);
	_turret->draw();
	glPopMatrix();
}

void SmallTank::castShadow(const Point &lightPostion) const
{
	_chassis->castShadow(&lightPostion);
}

void SmallTank::setNeighbors() const {
	_chassis->setNeighbors();
}


bool SmallTank::checkHit(Projectile* projectile_){

	Point projectilePosition = projectile_->getPosition();


	//compute maximum height of bounding box (max z point)

	float maxHeight;

	float tmp = std::max(_boundingBox->frontLeftUpper.z , _boundingBox->frontRightUpper.z);
	float tmp2 = std::max(_boundingBox->rearLeftUpper.z , _boundingBox->rearRightUpper.z);

	maxHeight = std::max(tmp, tmp2) + _position.z;


	//compute minimum height of bounding box (min z point)

	float minHeight;

	tmp = std::min(_boundingBox->frontLeftUnder.z , _boundingBox->frontRightUnder.z);
	tmp2 = std::min(_boundingBox->rearLeftUnder.z , _boundingBox->rearRightUnder.z);

	minHeight = std::min(tmp, tmp2) + _position.z;


	if(projectilePosition.z > maxHeight || projectilePosition.z < minHeight){

		return false;
	}

	//if programm reaches this point...the projectile has an appropriate height


	float max_x, min_x;


	//compute max x point
	tmp = std::max(_boundingBox->frontLeftUpper.x, _boundingBox->rearLeftUpper.x);
	tmp2 = std::max(_boundingBox->frontRightUpper.x, _boundingBox->rearRightUpper.x);

	max_x = std::max(tmp, tmp2);

	//compute min x point
	tmp = std::min(_boundingBox->frontLeftUpper.x, _boundingBox->rearLeftUpper.x);
	tmp2 = std::min(_boundingBox->frontRightUpper.x, _boundingBox->rearRightUpper.x);

	min_x = std::min(tmp, tmp2);


	if(projectilePosition.x > max_x || projectilePosition.x < min_x){
		return false;
	}


	float max_y, min_y;

	//copmute max y point

	tmp = std::max(_boundingBox->frontLeftUpper.y, _boundingBox->rearLeftUpper.y);
	tmp2 = std::max(_boundingBox->frontRightUpper.y, _boundingBox->rearRightUpper.y);

	max_y = std::max(tmp, tmp2);


	//compute min y point

	tmp = std::min(_boundingBox->frontLeftUpper.y, _boundingBox->rearLeftUpper.y);
	tmp2 = std::min(_boundingBox->frontRightUpper.y, _boundingBox->rearRightUpper.y);

	min_y = std::min(tmp, tmp2);

	if(projectilePosition.y > max_y || projectilePosition.y < min_y){
		return false;
	}else{
		return true;
	}

}

}
