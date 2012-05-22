/**
 * loadingScreen.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "loadingScreen.hpp"

// Includes
#include "defs.hpp"
#include "gl_includes.hpp"
#include "camera2D.hpp"
#include "introScreen.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

TGATexture* LoadingScreen::_texture;
Camera2D* LoadingScreen::_camera2D;
bool LoadingScreen::_on;
float LoadingScreen::_percentage;

LoadingScreen::LoadingScreen(Scene& scene) :
	_scene(scene) {
    _camera2D = NULL;
    _texture = NULL;
	_on = false;
	_percentage = 0.0;
}

LoadingScreen::~LoadingScreen() {
	delete _camera2D;
	delete _texture;
}

void LoadingScreen::initialize() {
	_camera2D = new Camera2D(_scene);
	_texture = new TGATexture("data/textures/loading.tga");
	_texture->init();
}

void LoadingScreen::draw() {
	if(!_on) {
		return;
	}
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

    _texture->setActive(true);

    glBegin(GL_QUADS);


	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, height, 0.0);

	glTexCoord2f(0.0, 768.0);
	glVertex3f(0.0, 0.0, 0.0);

	glTexCoord2f(1024.0, 768.0);
	glVertex3f(width, 0.0, 0.0);

	glTexCoord2f(1024.0, 0.0);
	glVertex3f(width, height, 0.0);


	glTexCoord2f(0.0, 768.0);
	glVertex3f((width / 2) - 256, (height / 2) + 256, 0.0);

	glTexCoord2f(0.0, 1024.0);
	glVertex3f((width / 2) - 256, (height / 2), 0.0);

	glTexCoord2f(512.0, 1024.0);
	glVertex3f((width / 2) + 256, (height / 2), 0.0);

	glTexCoord2f(512.0, 768.0);
	glVertex3f((width / 2) + 256, (height / 2) + 256, 0.0);


	glTexCoord2f(540.0, 790.0);
	glVertex3f((width / 2) - 210, (height / 2) - 100, 0.0);

	glTexCoord2f(540.0, 860.0);
	glVertex3f((width / 2) - 210, (height / 2) - 170, 0.0);

	glTexCoord2f(960.0, 860.0);
	glVertex3f((width / 2) + 210, (height / 2) - 170, 0.0);

	glTexCoord2f(960.0, 790.0);
	glVertex3f((width / 2) + 210, (height / 2) - 100, 0.0);


	glTexCoord2f(553.0, 893.0);
	glVertex3f((width / 2) - 197, (height / 2) - 113, 0.0);

	glTexCoord2f(553.0, 937.0);
	glVertex3f((width / 2) - 197, (height / 2) - 157, 0.0);

	glTexCoord2f(553.0 + (_percentage * 394.0), 937.0);
	glVertex3f((width / 2) - 197.0 + (_percentage * 394.0), (height / 2) - 157.0, 0.0);

	glTexCoord2f(553.0 + (_percentage * 394.0), 893.0);
	glVertex3f((width / 2) - 197.0 + (_percentage * 394.0), (height / 2) - 113.0, 0.0);


    glEnd();

    _texture->setActive(false);
    glPopMatrix();

    glutSwapBuffers();
}

bool LoadingScreen::isOn() {
	return _on;
}

void LoadingScreen::loadingFinished() {
	if(IntroScreen::isOn()) {
		return;
	}
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	_on = false;
}

void LoadingScreen::startLoading() {
	if(IntroScreen::isOn()) {
		return;
	}
	glutSetCursor(GLUT_CURSOR_NONE);
	_on = true;
	_percentage = 0.0;
	draw();
}

void LoadingScreen::setPercentage(float percentage) {
	_percentage = percentage;
	draw();
}

GRAPHICSLAB_NAMESPACE_END


