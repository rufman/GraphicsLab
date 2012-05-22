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
	_chassis = new PLYModel(_scene,400);
	_chassis->load(SMALLTANK_CHASSIS_MODEL);

	//get a new turret
	_turret = new Turret(_scene);

	int tankTextureNumber = 1;//rand()%3;
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
	default:
	{
		tankTexture = SMALLTANK_TEXTURE3;
		break;
	}
	}
	_tankTexture = new TGATexture(tankTexture.c_str());

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
	_tankTexture->setActive(true);
	_chassis->draw();
	_tankTexture->setActive(false);
	glPopMatrix();

	glPushMatrix();
	_turret->setRenderingParameters(_renderingParameters);
	Point tankPosition = getPosition();
	_turret->setPosition(Point(tankPosition.x,tankPosition.y,tankPosition.z));
	_tankTexture->setActive(true);
	_turret->draw();
	_tankTexture->setActive(false);
	glPopMatrix();
}

}
