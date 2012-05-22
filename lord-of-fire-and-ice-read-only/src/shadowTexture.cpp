/**
 * shadowTexture.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "defs.hpp"
#include "shadowTexture.hpp"
#include "utils.hpp"
#include "camera3D.hpp"
#include "map.hpp"
#include <cmath>
#include <cstdio>

GRAPHICSLAB_NAMESPACE_BEGIN

int ShadowTexture::_centerX;
int ShadowTexture::_centerY;
bool ShadowTexture::_on;
int ShadowTexture::_viewportWidth;
int ShadowTexture::_viewportHeight;
float ShadowTexture::_cameraFieldOfView;
uchar* ShadowTexture::_textureData;

ShadowTexture::ShadowTexture() :
	Texture() {
	_on = false;
	_viewportWidth = SIZE_OF_SHADOWED_SURFACE * SHADOW_PIXELS_PER_SQUARE;
	_viewportHeight = SIZE_OF_SHADOWED_SURFACE * SHADOW_PIXELS_PER_SQUARE;
	_cameraFieldOfView = Utils::toDegree(atan(((float)_viewportHeight / 4.0) / 10000.0));
	_textureData = new uchar[_viewportWidth * _viewportHeight];
}

ShadowTexture::~ShadowTexture() {
	delete [] _textureData;
}

void ShadowTexture::init() {
}


void ShadowTexture::grabScreen() {
    glBindTexture( _target, _id );
    glReadPixels(0, 0, _viewportWidth, _viewportHeight, GL_RED, GL_UNSIGNED_BYTE, _textureData);
    glTexImage2D(_target, 0, GL_ALPHA, _viewportWidth, _viewportHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, _textureData);
}

void ShadowTexture::saveTexture() {
    glBindTexture( _target, _id );
	GLint width, height;
	glGetTexLevelParameteriv( _target, 0, GL_TEXTURE_WIDTH, &width );
	glGetTexLevelParameteriv( _target, 0, GL_TEXTURE_HEIGHT, &height);
	uchar *data = new uchar[height*width*4];
	glGetTexImage( _target, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

	FILE* file = fopen("shadowTexture.raw", "wb");
	if(file != NULL) {
		for(int i = 0; i < (height * width); i++) {
			fwrite(&data[i * 4], 1, 1, file);
		}
		fclose(file);
	}
}

uchar *ShadowTexture::getImage() const {
    glBindTexture( _target, _id );
	GLint width, height;
	glGetTexLevelParameteriv( _target, 0, GL_TEXTURE_WIDTH, &width );
	glGetTexLevelParameteriv( _target, 0, GL_TEXTURE_HEIGHT, &height);
	uchar *data = new uchar[height*width*4];
	glGetTexImage( _target, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

	return data;
}


void ShadowTexture::setCenter(vec3f playerPosition) {
    vec3f nextSectorCorner = Map::getNextSectorCorner(playerPosition.x(), playerPosition.z());
    _centerX = nextSectorCorner.x();
    _centerY = nextSectorCorner.z();
}


int ShadowTexture::getViewportHeight() {
    return _viewportHeight;
}

int ShadowTexture::getViewportWidth() {
	return _viewportWidth;
}


void ShadowTexture::activateDrawing(bool on) {
	_on = on;
}


LookAt ShadowTexture::getCameraLookAt() {
    Point from(_centerX, -10000.0, _centerY);
    Point to(_centerX, 0.0, _centerY);
    return LookAt(from, to, vec3f(0.0, 0.0, 1.0));
}


float ShadowTexture::getCameraFieldOfView() {
    return _cameraFieldOfView;
}


bool ShadowTexture::isDrawing() {
	return _on;
}


int ShadowTexture::getCenterX() {
	return _centerX;
}

int ShadowTexture::getCenterY() {
	return _centerY;
}

float ShadowTexture::getShadowColor(float x, float y) {
	if(GraphicsSettings::getShadows() == 0) {
		return 1.0;
	}
	int intX = (int)floor((x - (float)(_centerX - (SIZE_OF_SHADOWED_SURFACE / 2))) * SHADOW_PIXELS_PER_SQUARE);
	int intY = (int)floor((y - (float)(_centerY - (SIZE_OF_SHADOWED_SURFACE / 2))) * SHADOW_PIXELS_PER_SQUARE);
	if(intX < 0.0 || intY < 0.0 || intX >= _viewportWidth || intY >= _viewportHeight) {
		return 1.0 - SHADOW_ALPHA;
	}
	if(_textureData[(intY * _viewportWidth) + intX] < 127) {
		return 1.0;
	} else {
		return 1.0 - SHADOW_ALPHA;
	}
}

GRAPHICSLAB_NAMESPACE_END

