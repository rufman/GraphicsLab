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
	_towerBase = new PLYModel(_scene,400);
	_towerBase->load(SMALLTOWER_TOWERBASE_MODEL,SMALLTANK_TEXTURE2);

	//get a new turret
	_turret = new Turret(_scene);

	int tankTextureNumber = rand() % 3;
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
	default: {
		tankTexture = SMALLTANK_TEXTURE3;
		break;
	}
	}
	_towerTexture = new TGATexture(tankTexture.c_str());

	reset();

}

SmallTower::~SmallTower() {
}

void SmallTower::draw() const {
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	_towerTexture->setActive(true);
	glTranslatef(_position.x, _position.y, _position.z);

	//a little bit higher over the ground because otherwise the model sinks into the ground
	glTranslatef(0,10, 0);
	glTranslatef(-1, 0, 2);

	Utils::applyGLRotation(Vector3D(0.0f, 1.0f, 0.0f), _scene.getTerrain().getNormal(_position));
	glScalef(_baseWidth * 15, _baseWidth * 15, _baseWidth * 15);
	glRotatef(-90, 1, 0, 0);

	_towerBase->setRenderingParameters(_renderingParameters);
	_towerBase->draw();
	_towerTexture->setActive(false);
	glPopMatrix();

	glPushMatrix();
	_towerTexture->setActive(true);
	_turret->setRenderingParameters(_renderingParameters);
	Point towerPosition = getPosition();
	_turret->setPosition(Point(towerPosition.x, towerPosition.y+13, towerPosition.z));
	_turret->draw();
	glPopMatrix();
	_towerTexture->setActive(false);
}

} /* namespace game_space */
