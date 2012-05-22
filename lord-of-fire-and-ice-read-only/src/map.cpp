/*
 * map.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 */


// Class declaration include
#include "map.hpp"

// Includes
#include "gl_includes.hpp"
#include "exception.hpp"
#include "utils.hpp"
#include "application.hpp"
#include "tgaTexture.hpp"
#include "sceneryObjectManager.hpp"
#include "sceneryObject.hpp"
#include "reflectionTexture.hpp"
#include "shadowTexture.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

GRAPHICSLAB_NAMESPACE_BEGIN


Map::Map(Scene& scene) :
	_scene(scene),
	_width(0),
	_height(0),
	_sectorsInX(0),
	_sectorsInY(0),
	_waterAnimationPosition(0.0),
	_sectors(NULL) 
{
	_surfaceTexture = new TGATexture("data/textures/surface.tga");
	_surfaceTexture->init();
	_reflectionTexture = new ReflectionTexture();
	_reflectionTexture->init();
	_shadowTexture = new ShadowTexture();
	_shadowTexture->init();
	_sceneryObjectManager = new SceneryObjectManager(this);
}


Map::~Map() {
	delete [] _sectors;
	delete _surfaceTexture;
	delete _reflectionTexture;
	delete _shadowTexture;
	delete _sceneryObjectManager;
}


bool Map::load(std::string directory) {
	
	delete [] _sectors;
	_sceneryObjectManager->reset();

	TGATexture* mapTexture;
	TGATexture* surfaceTexture;

    try {
    	mapTexture = new TGATexture(directory + "height_color.tga");
    } catch(const Exception &e) {
        return false;
    } catch(...) {
        return false;
    }

    try {
    	surfaceTexture = new TGATexture(directory + "surface.tga");
    } catch(const Exception &e) {
        return false;
    } catch(...) {
        return false;
    }

	TGAData* mapData = mapTexture->getData();
	TGAData* surfaceData = surfaceTexture->getData();

	_width = mapData->width;
	_height = mapData->height;
	
	srand(666); // The seed of the devil :P

	_randomNumbers = new int[_width * _height];
	_obstructingObjects = new int[_width *_height];
	
	_sectorsInX = (int)ceil((float)_width / (float)SECTOR_SIZE);
	_sectorsInY = (int)ceil((float)_height / (float)SECTOR_SIZE);

	_sectors = new Sector[_sectorsInX * _sectorsInY];

	int sectorWidth, sectorHeight;

	int loadingStep;

	loadingStep = _sectorsInY / 10;
	for(int sY = 0; sY < _sectorsInY; sY++) {
		if(sY % loadingStep == 0) {
			LoadingScreen::setPercentage(0.0 + (0.2 * ((float)sY / (float)_sectorsInY)));
		}
		for(int sX = 0; sX < _sectorsInX; sX++) {

			sectorWidth = SECTOR_SIZE + 1;
			if((((sX + 1) * SECTOR_SIZE) + 1) > _width) {
				sectorWidth = _width - (sX * SECTOR_SIZE);
			}

			sectorHeight = SECTOR_SIZE + 1;
			if((((sY + 1) * SECTOR_SIZE) + 1) > _height) {
				sectorHeight = _height - (sY * SECTOR_SIZE);
			}

			_sectors[(sY * _sectorsInY) + sX].init(this, sX * SECTOR_SIZE, sY * SECTOR_SIZE, sectorWidth, sectorHeight);
			_sectors[(sY * _sectorsInY) + sX].setSurfaceTexture(_surfaceTexture);
			_sectors[(sY * _sectorsInY) + sX].setReflectionTexture(_reflectionTexture);
			_sectors[(sY * _sectorsInY) + sX].setShadowTexture(_shadowTexture);
		}
	}

	float height;
	uchar red, green, blue, texture, sceneryObject;

	loadingStep = _height / 10;
	for(int y = 0; y < _height; y++) {
		if(y % loadingStep == 0) {
			LoadingScreen::setPercentage(0.2 + (0.2 * ((float)y / (float)_height)));
		}
		for(int x = 0; x < _width; x++) {

			height        = ((float)mapData->data[(((y * _width) + x) * 4) + 3]) * VERTICAL_SCALE;
			red           = mapData->data[(((y * _width) + x) * 4)];
			green         = mapData->data[(((y * _width) + x) * 4) + 1];
			blue          = mapData->data[(((y * _width) + x) * 4) + 2];
			texture       = surfaceData->data[(((y * _width) + x) * 4) + 3] / 64;
			sceneryObject = surfaceData->data[(((y * _width) + x) * 4)];

			setHeight(x, y, height);
			setColor(x, y, red, green, blue);
			setTexture(x, y, texture);
			if(sceneryObject > 0) {
				if(sceneryObject != 64) {
					Drawable* object = (Drawable*) new SceneryObject(_scene,_sceneryObjectManager, x, y, height, sceneryObject);
					addObject(object);
				}
				
				_obstructingObjects[y * _width + x] = _sceneryObjectManager->getObjectCollisionType(sceneryObject);// git 1 zruck wenn baum, und 2 wenn grosse stei, sunscht 0
			} else {
				_obstructingObjects[y * _width + x] = 0;
			}


			_randomNumbers[(y * _width) + x] = rand();
		}
	}

	vec3f* triangleNormals = new vec3f[(_width - 1) * (_height - 1) * 2];
	int t1, t2;
	Point point1, point2, point3, point4;

    loadingStep = (_height - 1) / 10;
    for(int y = 0; y < (_height - 1); y++) {
		if(y % loadingStep == 0) {
			LoadingScreen::setPercentage(0.4 + (0.2 * ((float)y / (float)(_height - 1))));
		}
        for(int x = 0; x < (_width - 1); x++) {
            t1 = (((y * (_width - 1)) + x) * 2);
            t2 = (((y * (_width - 1)) + x) * 2) + 1;

            point1 = getPoint(x    , y    );
            point2 = getPoint(x + 1, y    );
            point3 = getPoint(x    , y + 1);
            point4 = getPoint(x + 1, y + 1);

            triangleNormals[t1] = Utils::normal(point1, point3, point2);
            triangleNormals[t2] = Utils::normal(point2, point3, point4);
        }
    }

    vec3f normal;

    loadingStep = _height / 10;
	for(int y = 0; y < _height; y++) {
		if(y % loadingStep == 0) {
			LoadingScreen::setPercentage(0.6 + (0.2 * ((float)y / (float)_height)));
		}

		for(int x = 0; x < _width; x++) {

			normal = vec3f(0.0, 0.0, 0.0);

			if(x >= 1 && y >= 1) {
				normal += triangleNormals[2 * ((y - 1) * (_width - 1) + (x - 1)) + 1];
			}

			if(x < (_width - 1) && y >= 1) {
				normal += triangleNormals[2 * ((y - 1) * (_width - 1) +  x     )];
				normal += triangleNormals[2 * ((y - 1) * (_width - 1) +  x     ) + 1];
			}

			if(x >= 1 && y < (_height - 1)) {
				normal += triangleNormals[2 * ( y      * (_width - 1) + (x - 1))];
				normal += triangleNormals[2 * ( y      * (_width - 1) + (x - 1)) + 1];
			}

			if(x < (_width - 1) && y < (_height - 1)) {
				normal += triangleNormals[2 * ( y      * (_width - 1) +  x     )];
			}

			Utils::normalize(normal);

			setNormal(x, y, normal);
		}
	}

	loadingStep = (_sectorsInX * _sectorsInY) / 20;
	for(int i = 0; i < _sectorsInX * _sectorsInY; i++) {
		if(i % loadingStep == 0) {
			LoadingScreen::setPercentage(0.8 + (0.2 * ((float)i / (float)(_sectorsInX * _sectorsInY))));
		}
		_sectors[i].initDisplayLists();
	}

	delete mapTexture;
	delete mapData;
	delete [] triangleNormals;
	return true;
}


void Map::setHeight(int x, int y, float height) {
	int sectorX = x / SECTOR_SIZE;
	int sectorY = y / SECTOR_SIZE;

	_sectors[(sectorY * _sectorsInX) + sectorX].setHeight(x, y, height);

	if(x % SECTOR_SIZE == 0 && x != 0) {
		_sectors[(sectorY * _sectorsInX) + (sectorX - 1)].setHeight(x, y, height);
	}
	if(y % SECTOR_SIZE == 0 && y != 0) {
		_sectors[((sectorY - 1) * _sectorsInX) + sectorX].setHeight(x, y, height);
	}
	if(x % SECTOR_SIZE == 0 && x != 0 && y % SECTOR_SIZE == 0 && y != 0) {
		_sectors[((sectorY - 1) * _sectorsInX) + (sectorX - 1)].setHeight(x, y, height);
	}
}


void Map::setNormal(int x, int y, vec3f normal) {
	int sectorX = x / SECTOR_SIZE;
	int sectorY = y / SECTOR_SIZE;

	_sectors[(sectorY * _sectorsInX) + sectorX].setNormal(x, y, normal);

	if(x % SECTOR_SIZE == 0 && x != 0) {
		_sectors[(sectorY * _sectorsInX) + (sectorX - 1)].setNormal(x, y, normal);
	}
	if(y % SECTOR_SIZE == 0 && y != 0) {
		_sectors[((sectorY - 1) * _sectorsInX) + sectorX].setNormal(x, y, normal);
	}
	if(x % SECTOR_SIZE == 0 && x != 0 && y % SECTOR_SIZE == 0 && y != 0) {
		_sectors[((sectorY - 1) * _sectorsInX) + (sectorX - 1)].setNormal(x, y, normal);
	}
}


void Map::setTexture(int x, int y, uchar texture) {
	int sectorX = x / SECTOR_SIZE;
	int sectorY = y / SECTOR_SIZE;

	_sectors[(sectorY * _sectorsInX) + sectorX].setTexture(x, y, texture);

	if(x % SECTOR_SIZE == 0 && x != 0) {
		_sectors[(sectorY * _sectorsInX) + (sectorX - 1)].setTexture(x, y, texture);
	}
	if(y % SECTOR_SIZE == 0 && y != 0) {
		_sectors[((sectorY - 1) * _sectorsInX) + sectorX].setTexture(x, y, texture);
	}
	if(x % SECTOR_SIZE == 0 && x != 0 && y % SECTOR_SIZE == 0 && y != 0) {
		_sectors[((sectorY - 1) * _sectorsInX) + (sectorX - 1)].setTexture(x, y, texture);
	}
}


float Map::getHeight(int x, int y) {
	return getSectorI(x, y)->getHeight(x, y);
}


Point Map::getPoint(int x, int y) {
	return getSectorI(x, y)->getPoint(x, y);
}


void Map::setColor(int x, int y, uchar red, uchar green, uchar blue) {
	int sectorX = x / SECTOR_SIZE;
	int sectorY = y / SECTOR_SIZE;

	_sectors[(sectorY * _sectorsInX) + sectorX].setColor(x, y, red, green, blue);

	if(x % SECTOR_SIZE == 0 && x != 0) {
		_sectors[(sectorY * _sectorsInX) + (sectorX - 1)].setColor(x, y, red, green, blue);
	}
	if(y % SECTOR_SIZE == 0 && y != 0) {
		_sectors[((sectorY - 1) * _sectorsInX) + sectorX].setColor(x, y, red, green, blue);
	}
	if(x % SECTOR_SIZE == 0 && x != 0 && y % SECTOR_SIZE == 0 && y != 0) {
		_sectors[((sectorY - 1) * _sectorsInX) + (sectorX - 1)].setColor(x, y, red, green, blue);
	}
}

Color3f Map::getColor(int x, int y) {
	return getSectorI(x, y)->getColor(x, y);
}


Sector* Map::getSectorF(float x, float y) {
	if(x < 0.0 || x > (float)_width || y < 0.0 || y > (float)_height) {
        std::stringstream error;
        error << "getSectorF(): " << x << "/" << y << " is outside of map boundaries.";
        throw Exception(error.str());
	}
	return &_sectors[((int)floor(y / (float)SECTOR_SIZE) * _sectorsInX) + (int)floor(x / (float)SECTOR_SIZE)];
}


Sector* Map::getSectorI(int x, int y) {
	if(x < 0 || x > _width || y < 0 || y > _height) {
        std::stringstream error;
        error << "getSectorI(): " << x << "/" << y << " is outside of map boundaries.";
        throw Exception(error.str());
	}
	return &_sectors[((y / SECTOR_SIZE) * _sectorsInX) + (x / SECTOR_SIZE)];
}


int Map::getSectorID(float x, float y) {
	if(x < 0.0) {
		x = 0.0;
	}
	if(x >= _width) {
		x = _width - 1;
	}
	if(y < 0.0) {
		y = 0.0;
	}
	if(y >= _height) {
		y = _height - 1;
	}
	return ((int)floor(y / (float)SECTOR_SIZE) * _sectorsInX) + (int)floor(x / (float)SECTOR_SIZE);
}


Sector* Map::getSectorByID(int id) {
	if(id < 0 || id >= (_sectorsInX * _sectorsInY)) {
		return NULL;
	}
	return &_sectors[id];
}


void Map::draw(Player* _player) {

	vec3f cameraPosition = _player->getCameraPosition();
	float cameraAngle = _player->getCameraAngle();
	int cameraX = (int)cameraPosition.x() / SECTOR_SIZE;
	int cameraY = (int)cameraPosition.z() / SECTOR_SIZE;
	int sectorDistance;
	int quality;
	float azimuthToSector;
	float cameraToSector;

	bool drawSector[_sectorsInY * _sectorsInX];
	uchar sectorQuality[_sectorsInY * _sectorsInX];

	applyReflectionTransformation();

	for(int y = 0; y < _sectorsInY; y++) {
		for(int x = 0; x < _sectorsInX; x++) {
			drawSector[(y * _sectorsInY) + x] = false;
			sectorQuality[(y * _sectorsInY) + x] = 0;
			sectorDistance = (int) Utils::distance(cameraX, cameraY, x, y);
			if((ReflectionTexture::isDrawing() && sectorDistance < GraphicsSettings::getSectorReflectionRenderLimit()) ||
			   (!ReflectionTexture::isDrawing() && sectorDistance < GraphicsSettings::getSectorRenderLimit())) {

				azimuthToSector = Utils::getAzimuth(cameraX, cameraY, x, y);
				cameraToSector = Utils::angleDifference(cameraAngle, azimuthToSector);

				if((cameraToSector >= -135.0 && cameraToSector <= 135.0) || sectorDistance < 2) {
					quality = (1 + GraphicsSettings::getHighestQuality()) - (sectorDistance / 2);
					if(quality < 0) {
						quality = 0;
					}
					if(quality > GraphicsSettings::getHighestQuality()) {
						quality = GraphicsSettings::getHighestQuality();
					}
					if(ReflectionTexture::isDrawing() && quality > (GraphicsSettings::getHighestQuality() - 1)) {
						quality = (GraphicsSettings::getHighestQuality() - 1);
					}
					drawSector[(y * _sectorsInY) + x] = true;
					sectorQuality[(y * _sectorsInY) + x] = quality;
				}
			}
		}
	}
	for(int i = 0; i < (_sectorsInX * _sectorsInY); i++) {
		if(drawSector[i]) {
			_sectors[i].draw(sectorQuality[i]);
		}
	}
	glPushMatrix();
	if(!ShadowTexture::isDrawing() && GraphicsSettings::getShadows() == 1) {
		for(int i = 0; i < (_sectorsInX * _sectorsInY); i++) {
			if(drawSector[i] && sectorQuality[i] >= 2) {
				_sectors[i].drawShadow(sectorQuality[i]);
			}
		}
	}
	glPopMatrix();
	glPushMatrix();
	if(!ReflectionTexture::isDrawing()) {
		for(int i = 0; i < (_sectorsInX * _sectorsInY); i++) {
			if(drawSector[i]) {
				_sectors[i].drawWater(sectorQuality[i], _waterAnimationPosition);
			}
		}
	}
	glPopMatrix();
	removeReflectionTransformation();
}


void Map::addObject(Drawable* object) {
	getSectorF(object->x(), object->y())->addObject(object);
}


void Map::removeObject(Drawable* object) {
	getSectorF(object->x(), object->y())->removeObject(object);
}


float Map::getSurfaceHeight(float x, float y) {
	if(x < 0.0) {
		x = 0.0;
	}
	if(x >= _width) {
		x = _width - 1;
	}
	if(y < 0.0) {
		y = 0.0;
	}
	if(y >= _height) {
		y = _height - 1;
	}
	int intX = (int)floor(x);
	int intY = (int)floor(y);

	float deltaX = x - (float)intX;
	float deltaY = y - (float)intY;

	float height1 = getHeight(intX,     intY);
	float height2 = getHeight(intX + 1, intY);
	float height3 = getHeight(intX,     intY + 1);
	float height4 = getHeight(intX + 1, intY + 1);

	if(deltaX + deltaY < 1.0) {
		return height1 + deltaX * (height2 - height1) + deltaY * (height3 - height1);

	} else {
		return height4 + (1 - deltaX) * (height3 - height4) + (1 - deltaY) * (height2 - height4);

	}
}


int Map::getRandom(int x, int y) {
	return _randomNumbers[(y * _width) + x];
}


int Map::getMapWidth() {
	return _width;
}


int Map::getMapHeight() {
	return _height;
}


void Map::update(float duration) {
	_sceneryObjectManager->update(duration);
	_waterAnimationPosition += duration * 0.7854;
	if(_waterAnimationPosition > 6.2832) {
		_waterAnimationPosition -= 6.2832;
	}

}


vec3f Map::getNextSectorCorner(float x, float y) {
	return vec3f(SECTOR_SIZE * round(x / (float)SECTOR_SIZE), 0.0, SECTOR_SIZE * round(y / (float)SECTOR_SIZE));
}


void Map::grabReflectionTexture() {
	_reflectionTexture->grabScreen();
}


void Map::grabShadowTexture() {
	_shadowTexture->grabScreen();
}


void Map::applyReflectionTransformation() {
	if(ReflectionTexture::isDrawing()) {
		float waterAltitude = VERTICAL_SCALE * (float)LEVEL_OF_WATER;

	    GLdouble clipPlane[] = {0.0, -1.0, 0.0, waterAltitude - 0.30};
		glEnable(GL_CLIP_PLANE0);
		glClipPlane(GL_CLIP_PLANE0, clipPlane);

		glTranslatef(0.0, waterAltitude, 0.0);
		glScalef(1.0, -1.0, 1.0);
		glTranslatef(0.0, -waterAltitude, 0.0);

	}
}


void Map::removeReflectionTransformation() {
	if(ReflectionTexture::isDrawing()) {
		float waterAltitude = VERTICAL_SCALE * (float)LEVEL_OF_WATER;
		glDisable(GL_CLIP_PLANE0);
		glTranslatef(0.0, waterAltitude, 0.0);
		glScalef(1.0, -1.0, 1.0);
		glTranslatef(0.0, -waterAltitude, 0.0);
	}
}


void Map::applyShadowTransformation() {
	if(ShadowTexture::isDrawing()) {
		glRotatef(Sun::getAngle(), -1.0, 0.0, 0.0);
	}
}


void Map::removeShadowTransformation() {
	if(ShadowTexture::isDrawing()) {
		glRotatef(Sun::getAngle(), 1.0, 0.0, 0.0);
	}
}

int Map::getObstructingObjectI(int x, int y) {
	if(x < 0.0) {
		x = 0.0;
	}
	if(x >= _width) {
		x = _width - 1;
	}
	if(y < 0.0) {
		y = 0.0;
	}
	if(y >= _height) {
		y = _height - 1;
	}
	return _obstructingObjects[y * _width + x];
}

int Map::getObstructingObjectF(float x_, float y_) {
	int x = (int)round(x_);
	int y = (int)round(y_);
	if(x < 0.0) {
		x = 0.0;
	}
	if(x >= _width) {
		x = _width - 1;
	}
	if(y < 0.0) {
		y = 0.0;
	}
	if(y >= _height) {
		y = _height - 1;
	}
	return _obstructingObjects[y * _width + x];
}

void Map::setObstructingObjectI(int x, int y, int id) {
	if(x < 0.0) {
		x = 0.0;
	}
	if(x >= _width) {
		x = _width - 1;
	}
	if(y < 0.0) {
		y = 0.0;
	}
	if(y >= _height) {
		y = _height - 1;
	}
	_obstructingObjects[y * _width + x] = id;
}

void Map::setObstructingObjectF(float x_, float y_, int id) {
	int x = (int)round(x_);
	int y = (int)round(y_);
	if(x < 0.0) {
		x = 0.0;
	}
	if(x >= _width) {
		x = _width - 1;
	}
	if(y < 0.0) {
		y = 0.0;
	}
	if(y >= _height) {
		y = _height - 1;
	}
	_obstructingObjects[y * _width + x] = id;
}

GRAPHICSLAB_NAMESPACE_END
