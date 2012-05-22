/*
 * sceneryObjectManager.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "sceneryObjectManager.hpp"

// Includes
#include "defs.hpp"
#include "gl_includes.hpp"
#include "drawable.hpp"
#include "map.hpp"
#include "utils.hpp"
#include "shadowTexture.hpp"
#include <cmath>

GRAPHICSLAB_NAMESPACE_BEGIN


SceneryObjectManager::SceneryObjectManager(Map* map) :
	_map(map),

	_treeDisplayListsNum(16),
    _treeDisplayLists(glGenLists(_treeDisplayListsNum)),

	_grassDisplayListsNum(864),
    _grassDisplayLists(glGenLists(_grassDisplayListsNum)),

	_bushDisplayListsNum(4),
    _bushDisplayLists(glGenLists(_bushDisplayListsNum)),

	_stoneDisplayListsNum(4),
    _stoneDisplayLists(glGenLists(_stoneDisplayListsNum)),

    _duration(0.0001) { // possibility of division by _duration

	_objects = DrawableList();

	_treeTexture = new TGATexture[4];
	_treeTexture[0] = TGATexture("data/textures/trees0.tga");
	_treeTexture[0].init();
	_treeTexture[1] = TGATexture("data/textures/trees1.tga");
	_treeTexture[1].init();
	_treeTexture[2] = TGATexture("data/textures/trees2.tga");
	_treeTexture[2].init();
	_treeTexture[3] = TGATexture("data/textures/trees3.tga");
	_treeTexture[3].init();

	_grassBushTexture = new TGATexture("data/textures/grass_bush.tga");
	_grassBushTexture->init();

	_stoneTexture = new TGATexture[2];
	_stoneTexture[0] = TGATexture("data/textures/stone0.tga");
	_stoneTexture[0].init();
	_stoneTexture[1] = TGATexture("data/textures/stone1.tga");
	_stoneTexture[1].init();

	initDisplayLists();
}


SceneryObjectManager::~SceneryObjectManager() {
	delete [] _treeTexture;
	delete _grassBushTexture;
	delete [] _stoneTexture;
	reset();
}

void SceneryObjectManager::initDisplayLists() {

	//Trees

	float treeLeavesTextureOffsetX[] = {0.0, 192.0, 0.0, 192.0};
	float treeLeavesTextureOffsetY[] = {0.0, 0.0, 256.0, 256.0};

	float treeTrunkTextureOffsetX[] = {384.0, 448.0, 384.0, 448.0};
	float treeTrunkTextureOffsetY[] = {0.0, 0.0, 256.0, 256.0};

	float treeTrunkRadius[] = {0.15, 0.3, 0.2, 0.25};
	float treeTrunkHeight[] = {4.0, 2.2, 2.8, 2.1};

	float treeLeavesRadius[] = {3.0, 5.0, 3.0, 3.0};
	float treeLeavesBottom[] = {3.3, 1.0, 2.5, 1.5};
	float treeLeavesTop[]    = {11.3, 10.0, 10.5, 10.0};

	float treeTextureQualityScale[] = {1.0, 0.5, 0.25, 0.125};

	float sin22_5 = 0.382683;
	float cos22_5 = 0.923879;

	float textureX, textureY;

	for(int q = 0; q < 4; q++) {
		for(int tree = 0; tree < 4; tree++) {
			glNewList(_treeDisplayLists + (4 * tree) + q, GL_COMPILE);

			_treeTexture[q].setActive(true);
			glBegin(GL_QUADS);

			for(int i = 0; i < 8; i++) {
				glBegin(GL_QUADS);

				textureX = treeTrunkTextureOffsetX[tree] + 1.0 + (15.5 * (i % 4));
				textureX *= treeTextureQualityScale[q];
				textureY = treeTrunkTextureOffsetY[tree] + 1.0;
				textureY *= treeTextureQualityScale[q];
				glTexCoord2f(textureX, textureY);
				glNormal3f(-sin22_5, 0.0, cos22_5);
				glVertex3f(-sin22_5 * treeTrunkRadius[tree], treeTrunkHeight[tree], cos22_5 * treeTrunkRadius[tree]);

				textureX = treeTrunkTextureOffsetX[tree] + 1.0 + (15.5 * (i % 4));
				textureX *= treeTextureQualityScale[q];
				textureY = treeTrunkTextureOffsetY[tree] + 255.0;
				textureY *= treeTextureQualityScale[q];
				glTexCoord2f(textureX, textureY);
				glNormal3f(-sin22_5, 0.0, cos22_5);
				glVertex3f(-sin22_5 * treeTrunkRadius[tree], -0.5, cos22_5 * treeTrunkRadius[tree]);

				textureX = treeTrunkTextureOffsetX[tree] + 1.0 + (15.5 * ((i % 4) + 1));
				textureX *= treeTextureQualityScale[q];
				textureY = treeTrunkTextureOffsetY[tree] + 255.0;
				textureY *= treeTextureQualityScale[q];
				glTexCoord2f(textureX, textureY);
				glNormal3f( sin22_5, 0.0, cos22_5);
				glVertex3f( sin22_5 * treeTrunkRadius[tree], -0.5, cos22_5 * treeTrunkRadius[tree]);

				textureX = treeTrunkTextureOffsetX[tree] + 1.0 + (15.5 * ((i % 4) + 1));
				textureX *= treeTextureQualityScale[q];
				textureY = treeTrunkTextureOffsetY[tree] + 1.0;
				textureY *= treeTextureQualityScale[q];
				glTexCoord2f(textureX, textureY);
				glNormal3f( sin22_5, 0.0, cos22_5);
				glVertex3f( sin22_5 * treeTrunkRadius[tree], treeTrunkHeight[tree], cos22_5 * treeTrunkRadius[tree]);

				glEnd();

				glRotatef(45.0, 0.0, 1.0, 0.0);

			}

			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.5);
			glNormal3f(0.0, 1.0, 0.0);
			for(int i = 0; i < 2; i++) {

				glBegin(GL_QUADS);

				textureX = treeLeavesTextureOffsetX[tree] + 0.0;
				textureX *= treeTextureQualityScale[q];
				textureY = treeLeavesTextureOffsetY[tree] + 0.0;
				textureY *= treeTextureQualityScale[q];
				glTexCoord2f(textureX, textureY);
				glVertex3f(-treeLeavesRadius[tree], treeLeavesTop[tree], 0.0);

				textureX = treeLeavesTextureOffsetX[tree] + 192.0;
				textureX *= treeTextureQualityScale[q];
				textureY = treeLeavesTextureOffsetY[tree] + 0.0;
				textureY *= treeTextureQualityScale[q];
				glTexCoord2f(textureX, textureY);
				glVertex3f( treeLeavesRadius[tree], treeLeavesTop[tree], 0.0);

				textureX = treeLeavesTextureOffsetX[tree] + 192;
				textureX *= treeTextureQualityScale[q];
				textureY = treeLeavesTextureOffsetY[tree] + 256.0;
				textureY *= treeTextureQualityScale[q];
				glTexCoord2f(textureX, textureY);
				glVertex3f( treeLeavesRadius[tree], treeLeavesBottom[tree], 0.0);

				textureX = treeLeavesTextureOffsetX[tree] + 0.0;
				textureX *= treeTextureQualityScale[q];
				textureY = treeLeavesTextureOffsetY[tree] + 256.0;
				textureY *= treeTextureQualityScale[q];
				glTexCoord2f(textureX, textureY);
				glVertex3f(-treeLeavesRadius[tree], treeLeavesBottom[tree], 0.0);

				glEnd();

				glRotatef(90.0, 0.0, 1.0, 0.0);
			}
			glDisable(GL_ALPHA_TEST);
			glRotatef(180.0, 0.0, 1.0, 0.0);

			_treeTexture[q].setActive(false);

			glEndList();
		}
	}


	//Grass

	float grassTextureOffsetX[] = {0.5, 100.5, 200.5, 300.5, 0.5, 100.5};
	float grassTextureOffsetY[] = {0.5, 0.5, 0.5, 0.5, 100.5, 100.5};
	int grassTexture1, grassTexture2;
	float animationCos, animationSin;

	for(int grass = 0; grass < 6; grass++) {
		grassTexture1 = (grass % 3);
		grassTexture2 = (grass % 3) + 3;
		for(int animationStep = 0; animationStep < 144; animationStep++) {
			animationCos = 0.1 * cos((animationStep * 0.043633) - (grass * 0.5236));
			animationSin = 0.1 * sin((animationStep * 0.043633) - (grass * 0.5236));

			glNewList(_grassDisplayLists + (grass * 144) + animationStep, GL_COMPILE);
			glNormal3f(0.0, 1.0, 0.0);
			_grassBushTexture->setActive(true);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.5);

			glRotatef(grass * 30.0, 0.0, -1.0, 0.0);

			glBegin(GL_QUADS);

			glTexCoord2f(grassTextureOffsetX[grassTexture1], grassTextureOffsetY[grassTexture1] + 99.0);
			glVertex3f(-1.0 + animationCos, -1.0, 0.0 - animationSin);

			glTexCoord2f(grassTextureOffsetX[grassTexture1] + 99.0, grassTextureOffsetY[grassTexture1] + 99.0);
			glVertex3f( 1.0 + animationCos, -1.0, 0.0 - animationSin);

			glTexCoord2f(grassTextureOffsetX[grassTexture1] + 99.0, grassTextureOffsetY[grassTexture1]);
			glVertex3f( 1.0 - animationCos, 1.0, 0.0 + animationSin);

			glTexCoord2f(grassTextureOffsetX[grassTexture1], grassTextureOffsetY[grassTexture1]);
			glVertex3f(-1.0 - animationCos, 1.0, 0.0 + animationSin);

			glEnd();

			glRotatef(90.0, 0.0, -1.0, 0.0);

			glBegin(GL_QUADS);

			glTexCoord2f(grassTextureOffsetX[grassTexture2], grassTextureOffsetY[grassTexture2] + 99.0);
			glVertex3f(-1.0 - animationSin, -1.0, 0.0 - animationCos);

			glTexCoord2f(grassTextureOffsetX[grassTexture2] + 99.0, grassTextureOffsetY[grassTexture2] + 99.0);
			glVertex3f( 1.0 - animationSin, -1.0, 0.0 - animationCos);

			glTexCoord2f(grassTextureOffsetX[grassTexture2] + 99.0, grassTextureOffsetY[grassTexture2]);
			glVertex3f( 1.0 + animationSin, 1.0, 0.0 + animationCos);

			glTexCoord2f(grassTextureOffsetX[grassTexture2], grassTextureOffsetY[grassTexture2]);
			glVertex3f(-1.0 + animationSin, 1.0, 0.0 + animationCos);

			glEnd();

			glRotatef((grass * 30.0) + 90.0, 0.0, 1.0, 0.0);

			glDisable(GL_ALPHA_TEST);
			_grassBushTexture->setActive(false);
			glEndList();
		}
	}


	// Stone

	int stoneTexture[]          = {1, 1, 0, 0};
	int stoneSlices[]			= {12, 12, 16, 16};
	int stoneStacks[]			= {6, 6, 8, 8};
	float stoneTextureOffsetX[] = {0.5, 0.5, 1.0, 1.0};
	float stoneTextureOffsetY[] = {0.5, 32.5, 1.0, 129.0};
	float stoneTextureWidth[]   = {63.0, 63.0, 254.0, 254.0};
	float stoneTextureHeight[]  = {31.0, 31.0, 126.0, 126.0};

	for(int i = 0; i < 4; i++) {

		glNewList(_stoneDisplayLists + i, GL_COMPILE);

		_stoneTexture[stoneTexture[i]].setActive(true);

		glTranslatef(0.0, -0.1, 0.0);
		glRotatef(90, 0.0, 0.0, -1.0);

		Utils::drawTexturedSphere(1.0,
				stoneSlices[i],
				stoneStacks[i],
				stoneTextureOffsetX[i],
				stoneTextureOffsetY[i],
				stoneTextureWidth[i],
				stoneTextureHeight[i]);

		glRotatef(90, 0.0, 0.0, 1.0);
		glTranslatef(0.0, 0.1, 0.0);

		_stoneTexture[stoneTexture[i]].setActive(false);

		glEndList();
	}
}

void SceneryObjectManager::reset() {
	for(DrawableList::iterator i = _objects.begin(); i != _objects.end(); ++i) {
		delete (*i);
	}
	_objects.clear();
}

void SceneryObjectManager::addObject(Drawable* object) {
	_objects.push_back(object);
}


int SceneryObjectManager::getObjectCollisionType(int type) {
	if((type >= 252 && type <= 255) || type == 64) { // tree or invisible barrier
		return 1;
	}
	if(type >= 244 && type <= 245) { // big stone
		return 2;
	}
	return 0;
}


void SceneryObjectManager::draw(int x, int y, float altitude, int type, int quality, float& animationPosition) {


	if(ShadowTexture::isDrawing() && !(
	   (type >= 252 && type <= 255)
	   )) {
		return;
	}

	if(type >= 246 && type <= 251 && GraphicsSettings::getGrass() == 0) {
		return;
	}

	glTranslatef(x, altitude, y);
	Map::applyShadowTransformation();

	int random = _map->getRandom(x, y);
	// 251, 241, 239, 233 are all prime numbers
	float random1 = ((float)(random % 251)) / 250.0;
	float random2 = ((float)(random % 241)) / 240.0;
	float random3 = ((float)(random % 239)) / 238.0;
	float random4 = ((float)(random % 233)) / 232.0;

	if(type >= 252 && type <= 255) {
		/* tree
		 * random1: height and wind waving amplitude
		 * random2: width and wind waving frequency
		 * random3: color and wind waving frequency offset
		 * random4: rotation
		 */
		if(!ReflectionTexture::isDrawing() && !ShadowTexture::isDrawing()) {
			animationPosition += _duration * (20.0 + (40.0 * random2));
			if(animationPosition > 360.0) {
				animationPosition -= 360.0;
			}
		}
		float wavingAmplitude = 1.0 + (1.0 * (1 - random1));
		float wavingPosition = animationPosition + (360.0 * random3);

		float height = 0.8 + (0.7 * random1);
		float width = (0.9 + (0.2 * random2)) * height;

		glRotatef(wavingAmplitude * sin(Utils::toRadian(wavingPosition)), 0.0, 0.0, 1.0);
		glRotatef(wavingAmplitude * 0.5 * cos(Utils::toRadian(wavingPosition)), 1.0, 0.0, 0.0);
		glRotatef(360.0 * random4, 0.0, 1.0, 0.0);
		glScalef(width , height, width);

		if(!ShadowTexture::isDrawing()) {
			float color = 0.5 + (0.5 * random3);
			glColor3f(color, color, color);
		} else {
			glColor3f(0.0, 0.0, 0.0);
		}

		int displayListOffset = 0;
		if(type == 252) {
			displayListOffset = 0;
		}
		if(type == 253) {
			displayListOffset = 1;
		}
		if(type == 254) {
			displayListOffset = 2;
		}
		if(type == 255) {
			displayListOffset = 3;
		}

		glCallList(_treeDisplayLists + (displayListOffset * 4) + ( 3 - quality));

		glScalef(1.0 / width , 1.0 / height, 1.0 / width);

		glRotatef(360.0 * random4, 0.0, -1.0, 0.0);
		glRotatef(wavingAmplitude * 0.5 * cos(Utils::toRadian(wavingPosition)), -1.0, 0.0, 0.0);
		glRotatef(wavingAmplitude * sin(Utils::toRadian(wavingPosition)), 0.0, 0.0, -1.0);
	}

	if(type >= 246 && type <= 251) {
		/* grass
		 * random1: height
		 * random2: width
		 */
		if(quality == GraphicsSettings::getHighestQuality()) {
			if(!ReflectionTexture::isDrawing() && !ShadowTexture::isDrawing()) {
				animationPosition += _duration * 60.0;
				if(animationPosition > 360.0) {
					animationPosition -= 360.0;
				}
			}
			float height = 0.8 + (0.4 * random1);
			float width =  0.8 + (0.4 * random2);
			Color3f color = _map->getColor(x, y);
			float shadowColor = ShadowTexture::getShadowColor(x, y);
			glScalef(width, height, width);
			glColor3f(color.r * shadowColor, color.g * shadowColor, color.b * shadowColor);

			glCallList(_grassDisplayLists + ((type - 246) * 144) + (int)floor(animationPosition / 2.5));

			glScalef(1.0 / width , 1.0 / height, 1.0 / width);
		}
	}

	if(type >= 242 && type <= 243) {
		/* small stones
		 * random1: height
		 * random2: width
		 * random3: rotation in x
		 * random4: color and rotation in y
		 */
		if(quality == 3) {

			float height = 0.12 + (0.1 * random1);
			float width =  0.1 + (0.15 * random2);
			float rotateX = -15.0 + 30.0 * random3;
			float rotateY = 360.0 * random4;

			float color = (0.7 + (0.3 * random4)) * ShadowTexture::getShadowColor(x, y);
			glColor3f(color, color, color);
			glRotatef(rotateY, 0.0, 1.0, 0.0);
			glRotatef(rotateX, 1.0, 0.0, 0.0);
			glScalef(width , height, 0.18);

			glCallList(_stoneDisplayLists + (type - 242));

			glScalef(1.0 / width , 1.0 / height, 1.0 / 0.18);
			glRotatef(rotateX, -1.0, 0.0, 0.0);
			glRotatef(rotateY, 0.0, -1.0, 0.0);
		}
	}

	if(type >= 244 && type <= 245) {
		/* large stones
		 * random1: height
		 * random2: width
		 * random3: rotation in x
		 * random4: color and rotation in y
		 */
		if(quality == 2 || quality == 3 || quality == GraphicsSettings::getHighestQuality()) {

			float height = 0.4 + (0.3 * random1);
			float width =  0.3 + (0.6 * random2);
			float rotateX = -15.0 + 30.0 * random3;
			float rotateY = 360.0 * random4;

			float color = (0.7 + (0.3 * random4)) * ShadowTexture::getShadowColor(x, y);
			glColor3f(color, color, color);

			glRotatef(rotateY, 0.0, 1.0, 0.0);
			glRotatef(rotateX, 1.0, 0.0, 0.0);
			glScalef(width , height, 1.0);

			int textureQualityOffset = 0;
			if(quality == 3) {
				textureQualityOffset = 2;
			}

			glCallList(_stoneDisplayLists + (type - 244) + textureQualityOffset);

			glScalef(1.0 / width , 1.0 / height, 1.0);
			glRotatef(rotateX, -1.0, 0.0, 0.0);
			glRotatef(rotateY, 0.0, -1.0, 0.0);
		}
	}

	Map::removeShadowTransformation();
	glTranslatef(-x, -altitude, -y);
}

void SceneryObjectManager::update(float duration) {
	_duration = duration;
}


GRAPHICSLAB_NAMESPACE_END
