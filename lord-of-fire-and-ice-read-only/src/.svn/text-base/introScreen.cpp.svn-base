/**
 * introScreen.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "introScreen.hpp"

// Includes
#include "defs.hpp"
#include "gl_includes.hpp"
#include "camera2D.hpp";

GRAPHICSLAB_NAMESPACE_BEGIN

bool IntroScreen::_on;

IntroScreen::IntroScreen(Scene &scene) :
    _scene(scene),
    _camera2D(NULL),
    _mainMenuLoaded(false) {

	_on = true;
	glutSetCursor(GLUT_CURSOR_NONE);

	_camera2D = new Camera2D(_scene);

	_texture = new TGATexture("data/textures/intro_screen.tga");
	_texture->init();
}

IntroScreen::~IntroScreen() {
	delete _camera2D;
	delete _texture;
}

void IntroScreen::draw() {
	glClearColor(0.0, 0.0, 0.0, 0.0);

    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    Viewport viewport(0, 0, width, height);
    _camera2D->setViewport(viewport);

    _camera2D->applyViewport();
    _camera2D->applyProjection();
    _camera2D->applyModelview();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    int introImageX = 0;
    int introImageY = 0;
    int introImageWidth = 1024;
    int introImageHeight = 768;

    if((float)height / (float)width >= 0.75) {
		if(width > 1024) {
			introImageX = (width - 1024) / 2;
		} else if(width < 1024) {
			introImageWidth = width;
		}
		introImageHeight = (int)((float)introImageWidth * (3.0 / 4.0));
		introImageY = (height - introImageHeight) / 2;

    } else {
		if(height > 768) {
			introImageY = (height - 768) / 2;
		} else if(height < 768) {
			introImageHeight = height;
		}
		introImageWidth = (int)((float)introImageHeight * (4.0 / 3.0));
		introImageX = (width - introImageWidth) / 2;

    }

    _texture->setActive(true);

    glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(introImageX, introImageY + introImageHeight, 0.0);

	glTexCoord2f(0.0, 768.0);
	glVertex3f(introImageX, introImageY, 0.0);

	glTexCoord2f(1024.0, 768.0);
	glVertex3f(introImageX + introImageWidth, introImageY, 0.0);

	glTexCoord2f(1024.0, 0.0);
	glVertex3f(introImageX + introImageWidth, introImageY + introImageHeight, 0.0);

    glEnd();

    _texture->setActive(false);

    glPopMatrix();

    glutSwapBuffers();
}

void IntroScreen::update(float duration) {
	_duration += duration;
	if(_duration > INTRO_SCREEN_DISPLAY_LENGTH && _mainMenuLoaded) {
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		_on = false;
		GraphicsSettings::checkIfFullscreen();
	}
}

bool IntroScreen::isOn() {
	return _on;
}

void IntroScreen::loadingFinished() {
	_mainMenuLoaded = true;
}

GRAPHICSLAB_NAMESPACE_END


