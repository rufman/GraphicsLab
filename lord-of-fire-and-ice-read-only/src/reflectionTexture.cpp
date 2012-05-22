/**
 * reflectionTexture.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "defs.hpp"
#include "reflectionTexture.hpp"
#include "utils.hpp"
#include <cmath>

GRAPHICSLAB_NAMESPACE_BEGIN

bool ReflectionTexture::_on;
int ReflectionTexture::_viewportWidth;
float ReflectionTexture::_viewportRatio;

ReflectionTexture::ReflectionTexture() :
	Texture() {
	_on = false;
	_viewportWidth = 1;
	_viewportRatio = 1.0;
}

void ReflectionTexture::init() {
}


void ReflectionTexture::grabScreen() {
    glBindTexture( _target, _id );
	glCopyTexImage2D( _target, 0, GL_RGBA, 0, 0, _viewportWidth, VIEWPORT_HEIGHT, 0);
}

uchar *ReflectionTexture::getImage() const {
    glBindTexture( _target, _id );
	GLint width, height;
	glGetTexLevelParameteriv( _target, 0, GL_TEXTURE_WIDTH, &width );
	glGetTexLevelParameteriv( _target, 0, GL_TEXTURE_HEIGHT, &height);
	uchar *data = new uchar[height*width*4];
	glGetTexImage( _target, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

	return data;
}


int ReflectionTexture::getViewportHeight() {
    return VIEWPORT_HEIGHT;
}

int ReflectionTexture::getViewportWidth() {
	return _viewportWidth;
}

float ReflectionTexture::applyViewportRatioX(float& coord) {
	coord *= _viewportRatio;
	if(coord < 0) {
		coord = 0;
	} else if(coord > _viewportWidth) {
		coord = _viewportWidth;
	}
	return coord;
}

float ReflectionTexture::applyViewportRatioY(float& coord) {
	coord *= _viewportRatio;
	if(coord < 0) {
		coord = 0;
	} else if(coord > VIEWPORT_HEIGHT) {
		coord = VIEWPORT_HEIGHT;
	}
	return coord;
}


void ReflectionTexture::activateDrawing(bool on) {
	_on = on;
	if(on) {
	    int width = glutGet(GLUT_WINDOW_WIDTH);
	    int height = glutGet(GLUT_WINDOW_HEIGHT);
		_viewportRatio = (float)VIEWPORT_HEIGHT / (float)height;
		_viewportWidth = width * _viewportRatio;
	}
}

bool ReflectionTexture::isDrawing() {
	return _on;
}

GRAPHICSLAB_NAMESPACE_END

