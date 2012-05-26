/*
 * SmallTower.cpp
 */

#include "SmallTower.hpp"
#include "../../../common/GLIncludes.hpp"
#include "../../../common/Utils.hpp"
#include "../../Scene.hpp"
#include "../../Terrain.hpp"

namespace game_space {

SmallTower::SmallTower(Scene & scene, bool isAIControlled) :
		Tower(scene, isAIControlled) {
	_baseWidth = 1.0f;

	//create a new tower base
	_towerBase = new PLYModel(_scene);

	int towerTextureNumber = rand()%16;
	std::string towerTexture = "NO-TEXTURE-PATH";
	switch(towerTextureNumber)
	{
	case 0:
	{
		towerTexture = SMALLTANK_TEXTURE1;
		break;
	}
	case 1:
	{
		towerTexture = SMALLTANK_TEXTURE2;
		break;
	}
	case 2:
	{
		towerTexture = SMALLTANK_TEXTURE3;
		break;
	}
	case 3:
	{
		towerTexture = SMALLTANK_TEXTURE4;
		break;
	}
	case 4:
	{
		towerTexture = SMALLTANK_TEXTURE5;
		break;
	}
	case 5:
	{
		towerTexture = SMALLTANK_TEXTURE6;
		break;
	}
	case 6:
	{
		towerTexture = SMALLTANK_TEXTURE7;
		break;
	}
	case 7:
	{
		towerTexture = SMALLTANK_TEXTURE8;
		break;
	}
	case 8:
	{
		towerTexture = SMALLTANK_TEXTURE9;
		break;
	}
	case 9:
	{
		towerTexture = SMALLTANK_TEXTURE10;
		break;
	}
	case 10:
	{
		towerTexture = SMALLTANK_TEXTURE11;
		break;
	}
	case 11:
	{
		towerTexture = SMALLTANK_TEXTURE12;
		break;
	}
	case 12:
	{
		towerTexture = SMALLTANK_TEXTURE13;
		break;
	}
	case 13:
	{
		towerTexture = SMALLTANK_TEXTURE14;
		break;
	}
	case 14:
	{
		towerTexture = SMALLTANK_TEXTURE15;
		break;
	}
	case 15:
	{
		towerTexture = SMALLTANK_TEXTURE16;
		break;
	}
	default:
	{
		towerTexture = SMALLTANK_TEXTURE3;
		break;
	}
	}
	_towerBase->load(SMALLTOWER_TOWERBASE_MODEL,towerTexture.c_str());

	//get a new turret
	_turret = new Turret(_scene);
	_turret->load(TURRET_MODEL,towerTexture.c_str());
	reset();

}

SmallTower::~SmallTower() {
}

void SmallTower::draw() const {
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	//_towerTexture->setActive(true);
	glTranslatef(_position.x, _position.y, _position.z);

	//a little bit higher over the ground because otherwise the model sinks into the ground
	glTranslatef(0,10, 0);
	//glTranslatef(-1, 0, 2);

	Utils::applyGLRotation(Vector3D(0.0f, 0.0f, 1.0f), _scene.getTerrain().getNormal(_position));
	glScalef(_baseWidth * 15, _baseWidth * 15, _baseWidth * 15);

	_towerBase->setRenderingParameters(_renderingParameters);
	_towerBase->draw();

	//translate the turret to the right position
	glTranslatef(0,0.23, 0.32);

	glScalef(_baseWidth /4, _baseWidth /4, _baseWidth /4);

	_turret->setRenderingParameters(_renderingParameters);
	Point towerPosition = getPosition();
	_turret->setPosition(Point(towerPosition.x, towerPosition.y+_baseWidth * 15, towerPosition.z));
	_turret->draw();
	glPopMatrix();
	//_towerTexture->setActive(false);
}

} /* namespace game_space */
