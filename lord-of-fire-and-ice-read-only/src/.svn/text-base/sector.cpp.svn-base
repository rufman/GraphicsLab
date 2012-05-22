/*
 * sector.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "sector.hpp"

// Includes
#include "defs.hpp"
#include "gl_includes.hpp"
#include "map.hpp"
#include "utils.hpp"
#include "reflectionTexture.hpp"
#include "shadowTexture.hpp"
#include <cmath>
#include <iostream>

GRAPHICSLAB_NAMESPACE_BEGIN

Sector::Sector() :
	_offsetX(0),
	_offsetY(0),
	_width(0),
	_height(0),
	_hasWater(false),
    _numDisplayLists(5),
    _displayLists(glGenLists(_numDisplayLists)),
	_heightMap(NULL),
	_colorMap(NULL),
	_normals(NULL),
	_objects(NULL),
	_texture(NULL) {
}


Sector::~Sector() {
	glDeleteLists(_displayLists, _numDisplayLists);

	delete [] _heightMap;
	delete [] _colorMap;
	delete [] _normals;
	delete [] _texture;
}

void Sector::init(Map* map, int offsetX, int offsetY, int width, int height) {
	_map = map;
	_offsetX = offsetX;
	_offsetY = offsetY;
	_width = width;
	_height = height;
	_heightMap = new float[_width * _height];
	_colorMap = new Color3f[_width * _height];
	_normals = new vec3f[_width * _height];
	_texture = new uchar[_width * _height];
	_objects = DrawableList();
}

void Sector::setHeight(int x, int y, float height) {
	x -= _offsetX;
	y -= _offsetY;
	_heightMap[(y * _width) + x] = height;
}

void Sector::setNormal(int x, int y, vec3f normal) {
	x -= _offsetX;
	y -= _offsetY;
	_normals[(y * _width) + x] = normal;
}

void Sector::setTexture(int x, int y, uchar texture) {
	x -= _offsetX;
	y -= _offsetY;
	_texture[(y * _width) + x] = texture;
}

float Sector::getHeight(int x, int y) {
	x -= _offsetX;
	y -= _offsetY;
	return _heightMap[(y * _width) + x];
}

Point Sector::getPoint(int x, int y) {
	return Point((float)x, _heightMap[((y - _offsetY) * _width) + (x - _offsetX)], (float)y);
}

void Sector::setColor(int x, int y, uchar red, uchar green, uchar blue) {
	x -= _offsetX;
	y -= _offsetY;
	float redF   = ((float)red  ) / 255;
	float greenF = ((float)green) / 255;
	float blueF  = ((float)blue ) / 255;
	_colorMap[(y * _width) + x] = Color3f(redF, greenF, blueF);
}

Color3f Sector::getColor(int x, int y) {
	x -= _offsetX;
	y -= _offsetY;
	return _colorMap[(y * _width) + x];
}

void Sector::initDisplayLists() {

	int quality_steps[] = {8,4,2,1};

	int index1, index2, index3, index4;
	int texture, textureX, textureY;
	int d, x, y;

	for(int i = 0; i < 4; i++) {

		d = quality_steps[i];

		glNewList(_displayLists + i, GL_COMPILE);

		_surfaceTexture->setActive(true);


		glBegin(GL_TRIANGLES);

		for(y = d; y < _height; y += d) {
			for(x = d; x < _width; x += d) {

				index1 = ((y - d) * _width) + (x - d);
				index2 = ((y - d) * _width) +  x;
				index3 = ( y      * _width) + (x - d);
				index4 = ( y      * _width) +  x;

				texture  = _texture[index1] * 64;
				texture += _texture[index2] * 16;
				texture += _texture[index3] * 4;
				texture += _texture[index4];

				textureX = (texture % 16) * SURFACE_TEXTURE_SIZE;
				textureY = (texture / 16) * SURFACE_TEXTURE_SIZE;


				glTexCoord2f(textureX, textureY);
				glColor3f(_colorMap[index1].r, _colorMap[index1].g, _colorMap[index1].b);
				glNormal3f(_normals[index1].x(), _normals[index1].y(), _normals[index1].z());
				glVertex3f(x + _offsetX - d, _heightMap[index1], y + _offsetY - d);

				glTexCoord2f(textureX + SURFACE_TEXTURE_SIZE, textureY);
				glColor3f(_colorMap[index2].r, _colorMap[index2].g, _colorMap[index2].b);
				glNormal3f(_normals[index2].x(), _normals[index2].y(), _normals[index2].z());
				glVertex3f(x + _offsetX, _heightMap[index2], y + _offsetY - d);

				glTexCoord2f(textureX, textureY + SURFACE_TEXTURE_SIZE);
				glColor3f(_colorMap[index3].r, _colorMap[index3].g, _colorMap[index3].b);
				glNormal3f(_normals[index3].x(), _normals[index3].y(), _normals[index3].z());
				glVertex3f(x + _offsetX - d, _heightMap[index3], y + _offsetY);

				glTexCoord2f(textureX + SURFACE_TEXTURE_SIZE, textureY);
				glColor3f(_colorMap[index2].r, _colorMap[index2].g, _colorMap[index2].b);
				glNormal3f(_normals[index2].x(), _normals[index2].y(), _normals[index2].z());
				glVertex3f(x + _offsetX, _heightMap[index2], y + _offsetY - d);

				glTexCoord2f(textureX + SURFACE_TEXTURE_SIZE, textureY + SURFACE_TEXTURE_SIZE);
				glColor3f(_colorMap[index4].r, _colorMap[index4].g, _colorMap[index4].b);
				glNormal3f(_normals[index4].x(), _normals[index4].y(), _normals[index4].z());
				glVertex3f(x + _offsetX, _heightMap[index4], y + _offsetY);

				glTexCoord2f(textureX, textureY + SURFACE_TEXTURE_SIZE);
				glColor3f(_colorMap[index3].r, _colorMap[index3].g, _colorMap[index3].b);
				glNormal3f(_normals[index3].x(), _normals[index3].y(), _normals[index3].z());
				glVertex3f(x + _offsetX - d, _heightMap[index3], y + _offsetY);

			}

		}

		glEnd();

		_surfaceTexture->setActive(false);

		/* --- NORMAL DEBUGGING ---

		glDisable(GL_LIGHTING);

		glBegin(GL_LINES);

		for(int y = 0; y < _height; y++) {
			for(int x = 0; x < _width; x++) {
				index1 = (y * _width) + x;

				glColor3f(1.0, 0.0, 0.0);
				glVertex3f(x + _offsetX, _heightMap[index1], y + _offsetY);
				glVertex3f(x + _offsetX + _normals[index1].x(), _heightMap[index1] +  _normals[index1].y() + 1.0, y + _offsetY + _normals[index1].z());
			}

		}

		glEnd();

		glEnable(GL_LIGHTING);

		*/


		if(i < 3) {
			glNormal3f(0.0, 1.0, 0.0);
			glBegin(GL_QUAD_STRIP);

			x = 0;
			for(y = 0; y < _height; y += d) {
				curtainVertices(x, y);
			}
			y -= d;
			for(x = d; x < _width; x += d) {
				curtainVertices(x, y);
			}
			glEnd();
			glBegin(GL_QUAD_STRIP);
			y = 0;
			for(x = 0; x < _width; x += d) {
				curtainVertices(x, y);
			}
			x -= d;
			for(y = d; y < _height; y += d) {
				curtainVertices(x, y);
			}

			glEnd();
		}

		glEndList();
	}

	// Check if water should be drawn for sector
	for(int i = 0; i < (_width * _height); i++) {
		if(_heightMap[i] < Map::LEVEL_OF_WATER) {
			_hasWater = true;
		}
	}
}

void Sector::curtainVertices(int x, int y) {
	int index = (y * _width) + x;
	glColor3f(_colorMap[index].r * 0.9, _colorMap[index].g * 0.9, _colorMap[index].b * 0.9);
	glVertex3f(x + _offsetX, _heightMap[index], y + _offsetY);
	glVertex3f(x + _offsetX, _heightMap[index] - 2.0, y + _offsetY);
}

void Sector::draw(int quality) {
	if(!ShadowTexture::isDrawing()) {
		glPushMatrix();
		glCallList(_displayLists + quality);
		glPopMatrix();
	}
	glPushMatrix();
	if(!ShadowTexture::isDrawing() || quality >= 1) {
		for(DrawableList::iterator i = _objects.begin(); i != _objects.end(); ++i) {
			(*i)->draw(quality);
		}
	}
	glPopMatrix();
}

void Sector::drawShadow(int quality) {

	int index1, index2, index3, index4;
	int x, y;

	float shadowRaise = 0.002;

	int surfaceQuadSizes[] = {8, 4, 2, 1};
	int d = surfaceQuadSizes[quality];

	int shadowPixelPerSquare = ShadowTexture::SHADOW_PIXELS_PER_SQUARE;
	int shadowTextureRadius = ShadowTexture::SIZE_OF_SHADOWED_SURFACE / 2;
	int shadowTextureOffsetX = (_offsetX - (ShadowTexture::getCenterX() - shadowTextureRadius)) * shadowPixelPerSquare;
	int shadowTextureOffsetY = (_offsetY - (ShadowTexture::getCenterY() - shadowTextureRadius)) * shadowPixelPerSquare;

	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_shadowTexture->setActive(true);

	glColor4f(0.0, 0.0, 0.0, ShadowTexture::SHADOW_ALPHA);

	glBegin(GL_TRIANGLES);

	for(y = d; y < _height; y++) {
		for(x = d; x < _width; x++) {

			index1 = ((y - d) * _width) + (x - d);
			index2 = ((y - d) * _width) +  x;
			index3 = ( y      * _width) + (x - d);
			index4 = ( y      * _width) +  x;

			glTexCoord2f(shadowTextureOffsetX + ((x - d) * shadowPixelPerSquare), shadowTextureOffsetY + ((y - d) * shadowPixelPerSquare));
			glVertex3f(x + _offsetX - d, _heightMap[index1] + shadowRaise, y + _offsetY - d);

			glTexCoord2f(shadowTextureOffsetX + ( x      * shadowPixelPerSquare), shadowTextureOffsetY + ((y - d) * shadowPixelPerSquare));
			glVertex3f(x + _offsetX, _heightMap[index2] + shadowRaise, y + _offsetY - d);

			glTexCoord2f(shadowTextureOffsetX + ((x - d) * shadowPixelPerSquare), shadowTextureOffsetY + ( y      * shadowPixelPerSquare));
			glVertex3f(x + _offsetX - d, _heightMap[index3] + shadowRaise, y + _offsetY);

			glTexCoord2f(shadowTextureOffsetX + ( x      * shadowPixelPerSquare), shadowTextureOffsetY + ((y - d) * shadowPixelPerSquare));
			glVertex3f(x + _offsetX, _heightMap[index2] + shadowRaise, y + _offsetY - d);

			glTexCoord2f(shadowTextureOffsetX + ( x      * shadowPixelPerSquare), shadowTextureOffsetY + ( y      * shadowPixelPerSquare));
			glVertex3f(x + _offsetX, _heightMap[index4] + shadowRaise, y + _offsetY);

			glTexCoord2f(shadowTextureOffsetX + ((x - d) * shadowPixelPerSquare), shadowTextureOffsetY + ( y      * shadowPixelPerSquare));
			glVertex3f(x + _offsetX - d, _heightMap[index3] + shadowRaise, y + _offsetY);

		}

	}

	glEnd();

	_shadowTexture->setActive(false);

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);

}

void Sector::drawWater(int quality, float waterAnimationPosition) {
	if(!_hasWater || ShadowTexture::isDrawing()) {
		return;
	}

	int surfaceQuadSizes[] = {16, 16, 8, 4};
	int d = surfaceQuadSizes[quality];

	int waterWidth  = ((_width - 1)  / d) + 1;
	int waterHeight = ((_height - 1)  / d) + 1;

	float vertexX[waterWidth * waterHeight];
	float vertexY[waterWidth * waterHeight];
	float vertexZ[waterWidth * waterHeight];

	float textureX[waterWidth * waterHeight];
	float textureY[waterWidth * waterHeight];

	float random1;
	float random2;
	float random3;
	vec3f onScreen;

	float waterAltitude = Map::VERTICAL_SCALE * (float)Map::LEVEL_OF_WATER;

	for(int y = 0; y < waterHeight; y++) {
		for(int x = 0; x < waterWidth; x++) {

			int random = _map->getRandom((x * d) + _offsetX, (y * d) + _offsetY);
			// 251, 241, 239 are all prime numbers
			random1 = ((float)(random % 251)) / 250.0;
			random2 = ((float)(random % 241)) / 240.0;
			random3 = ((float)(random % 239)) / 238.0;

			vertexX[x + (y * waterWidth)] = (x * d) + _offsetX + (0.2 * sin(waterAnimationPosition + (6.2832 * random1)));
			vertexY[x + (y * waterWidth)] = waterAltitude + (0.05 * sin(waterAnimationPosition + (6.2832 * random2)));
			vertexZ[x + (y * waterWidth)] = (y * d) + _offsetY + (0.2 * sin(waterAnimationPosition + (6.2832 * random3)));

			onScreen = Utils::worldToScreen((x * d) + _offsetX, waterAltitude, (y * d) + _offsetY);
			textureX[x + (y * waterWidth)] = ReflectionTexture::applyViewportRatioX(onScreen.x());
			textureY[x + (y * waterWidth)] = ReflectionTexture::applyViewportRatioY(onScreen.y());
		}
	}
	int index1, index2, index3, index4;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(GraphicsSettings::getReflections()) {
		_reflectionTexture->setActive(true);
		glColor4f(1.0, 1.0, 1.0, 0.6);
	} else {
		Color3f skyColor = Scene::getBackgroundColor();
		glColor4f(skyColor.r * 0.5, skyColor.g * 0.5, skyColor.b * 0.5, 0.6);
	}
	glBegin(GL_TRIANGLES);

	glNormal3f(0.0, 1.0, 0.0);
	for(int y = 1; y < waterHeight; y++) {
		for(int x = 1; x < waterWidth; x++) {

			index1 = (x - 1) + ((y - 1) * waterWidth);
			index2 =  x + ((y - 1) * waterWidth);
			index3 = (x - 1) + (y * waterWidth);
			index4 =  x + (y * waterWidth);
			glTexCoord2f(textureX[index1], textureY[index1]);
			glVertex3f(vertexX[index1], vertexY[index1], vertexZ[index1]);

			glTexCoord2f(textureX[index2], textureY[index2]);
			glVertex3f(vertexX[index2], vertexY[index2], vertexZ[index2]);

			glTexCoord2f(textureX[index3], textureY[index3]);
			glVertex3f(vertexX[index3], vertexY[index3], vertexZ[index3]);

			glTexCoord2f(textureX[index2], textureY[index2]);
			glVertex3f(vertexX[index2], vertexY[index2], vertexZ[index2]);

			glTexCoord2f(textureX[index4], textureY[index4]);
			glVertex3f(vertexX[index4], vertexY[index4], vertexZ[index4]);

			glTexCoord2f(textureX[index3], textureY[index3]);
			glVertex3f(vertexX[index3], vertexY[index3], vertexZ[index3]);
		}

	}

	glEnd();
	if(GraphicsSettings::getReflections()) {
		_reflectionTexture->setActive(false);
	}
	glDisable(GL_BLEND);
}

void Sector::addObject(Drawable* object) {
	for(DrawableList::iterator i = _objects.begin(); i != _objects.end(); ++i) {
		if(*i == object) {
			return;
		}
	}
	_objects.push_back(object);
}

void Sector::removeObject(Drawable* object) {
	for(DrawableList::iterator i = _objects.begin(); i != _objects.end(); ++i) {
		if(*i == object) {
			_objects.erase(i);
			return;
		}
	}
}

void Sector::setSurfaceTexture(TGATexture* surfaceTexture) {
	_surfaceTexture = surfaceTexture;
}

void Sector::setReflectionTexture(ReflectionTexture* reflectionTexture) {
	_reflectionTexture = reflectionTexture;
}

void Sector::setShadowTexture(ShadowTexture* shadowTexture) {
	_shadowTexture = shadowTexture;
}

GRAPHICSLAB_NAMESPACE_END
