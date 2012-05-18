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

SmallTank::SmallTank(Scene & scene,int id,TankAI* ai):Tank(scene,id,ai) {
	_baseWidth = 1.0f;

	//create a new chassis
	_chassis = new PLYModel(_scene);
	_chassis->load(SMALLTANK_CHASSIS_MODEL);

	//get a new turret
	_turret = new Turret(_scene);

	reset();

}

SmallTank::~SmallTank() {}

void SmallTank::draw() const
{
	glTranslatef(_position.x,_position.y,_position.z);

	//a little bit higher over the ground because otherwise the model sinks into the ground
	glTranslatef(0,2,0);

	Utils::applyGLRotation(Vector3D(0.0f,1.0f,0.0f),_scene.getTerrain().getNormal(_position));

	glScalef(_baseWidth*5,_baseWidth*5,_baseWidth*5);
	glRotatef(-90,1,0,0);
	glRotatef(-getDirection(),0,0,1);
	glTranslatef(0,-0.5,0);

	_chassis->setRenderingParameters(_renderingParameters);
	_chassis->draw();

	//clear operations from above
	glTranslatef(0,0.5,0);
	glRotatef(getDirection(),0,0,1);
	glRotatef(90,1,0,0);

	glRotatef( -_turret->getAzimuth()+180, 0.0, 1.0, 0.0 );
	glScalef(_baseWidth*0.5,_baseWidth*0.5,_baseWidth*0.5);
	_turret->setRenderingParameters(_renderingParameters);
	_turret->draw();
}

}
