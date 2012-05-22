/**
 * scene.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "scene.hpp"

// Includes
#include "gl_includes.hpp"
#include "exception.hpp"
#include "utils.hpp"
#include "application.hpp"
#include "tgaTexture.hpp"
#include "reflectionTexture.hpp"
#include "shadowTexture.hpp"
#include "camera2D.hpp"
#include "camera3D.hpp"
#include "map.hpp"
#include "player.hpp"
#include "inputManager.hpp"
#include "sky.hpp"
#include "map.hpp"
#include "fog.hpp"
#include "inputManager.hpp"
#include "introScreen.hpp"
#include "loadingScreen.hpp"
#include "menuManager.hpp"
#include "sun.hpp"
#include "graphicsSettings.hpp"
#include "gameSaveFunctions.hpp"


#include <cstdlib>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>



GRAPHICSLAB_NAMESPACE_BEGIN	



Color3f Scene::_backgroundColor;

Scene::Scene(Window &window) :
    _window(window),
    _map(NULL),
    _player(NULL),
    _fog(NULL),
    _camera2D(NULL),
    _camera3D(NULL),
	_sun( NULL ),
	_introScreen(NULL),
	_loadingScreen(NULL),
	_dock(NULL),
	_graphicsSettings(NULL),
	_npcManager(NULL)
{

}

Scene::~Scene() {
    delete _camera2D;
    delete _camera3D;

    delete _sun;

    delete _map;
    delete _player;

    delete _introScreen;

    delete _inputManager;
    delete _menuManager;

    delete _graphicsSettings;

    delete _dock;

    delete _npcManager;
}

void Scene::initialize() {

    // Anti-aliasing
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_POLYGON_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    // Normalize vectors automatically before being processed
    glEnable( GL_NORMALIZE );

    // This sets the texture environment for combining texture image
    // with the lighting calculations.
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	// Intro screen
	_introScreen = new IntroScreen(*this);
	_introScreen->draw();

	// Loading screen
	_loadingScreen = new LoadingScreen(*this);

	reset();

	postIntroInitialize();
}


void Scene::postIntroInitialize() {

	_loadingScreen->initialize();

	_inputManager = new InputManager();

	_menuManager = new MenuManager(*this);
	_menuManager->initialize();
	
	_dock = new Dock(*this);

	_graphicsSettings = new GraphicsSettings(&_window);
	_graphicsSettings->load();

	_map = new Map(*this);

	_player = new Player(*this, _map);
	_player->init();

	_npcManager = new NPCManager(*this, _map, _player);
	_npcManager->init();

	_fog = new Fog(*this);

	_backgroundColor = Color3f(0.66, 0.83, 0.96);
	
    _camera2D = new Camera2D(*this);
    _camera3D = new Camera3D(*this);

	_sky = new Sky ( _map ) ;
	
    _sun = new Sun(*this, _map);

	float globalAmbientArray[4] = { 0.3, 0.3, 0.3, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientArray);

	_gameSaveFunctions = new GameSaveFunctions(*this, _loadingScreen, _map, _sun, _player, _npcManager);
	_gameSaveFunctions->loadIntroMap();

	_introScreen->loadingFinished();
}

void Scene::setClearColor() {
	glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, 1.0);
}

void Scene::reset() {
    _updateTimer.start();
}

void Scene::onPaint() {

	if(_introScreen->isOn()) {
		_introScreen->draw();
		return;
	}
	if(_loadingScreen->isOn()) {
		_loadingScreen->draw();
		return;
	}

    // Generate shadow texture;
    drawShadowTexture();

    // Generate reflection texture;
    drawReflectionTexture();

    // Draw game world
	drawWorld();

	// Draw Interface & Dock
    drawInterface();

    // Swap buffers
    glutSwapBuffers();
}


void Scene::drawWorld() {
	setClearColor();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    Viewport viewport(0, 0, width, height);
    _camera3D->setViewport(viewport);

	if(!DEBUG_MAP) {
		_camera3D->setLookAt(_player->getCameraLookAt());
	} else {
	    Point from(_map->getMapWidth() / 2, 600.0, _map->getMapHeight() / 2);
	    Point to(_map->getMapWidth() / 2, 0.0, _map->getMapHeight() / 2);
	    _camera3D->setLookAt(LookAt( from, to, vec3f( 0.0, 0.0, 1.0 ) ));
	}
	_camera3D->setFieldOfView(90.0);


    _camera3D->applyViewport();
    _camera3D->applyProjection();
    _camera3D->applyModelview();

	glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_FOG);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	
    _sun->apply();

	if(!DEBUG_MAP) {
		_fog->apply();
	}

    glPopMatrix();

    if(!DEBUG_MAP) {
		_sun->draw();
		_sky->draw ( ) ;
    }

    _map->draw(_player);

    _player->draw();
}


void Scene::drawShadowTexture() {
	if(GraphicsSettings::getShadows() == 0) {
		return;
	}

    glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    ShadowTexture::activateDrawing(true);
    ShadowTexture::setCenter(_player->getPosition());

    Viewport viewport(0, 0,
    		          ShadowTexture::getViewportWidth(),
    		          ShadowTexture::getViewportHeight());
    _camera3D->setViewport(viewport);
	_camera3D->setLookAt(ShadowTexture::getCameraLookAt());
	_camera3D->setFieldOfView(ShadowTexture::getCameraFieldOfView());

    _camera3D->applyViewport();
    _camera3D->applyProjection();
    _camera3D->applyModelview();

	glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_COLOR_LOGIC_OP);
    glLogicOp(GL_COPY_INVERTED);

    _map->draw(_player);

    _player->draw();

    _map->grabShadowTexture();

    glDisable(GL_COLOR_LOGIC_OP);

    ShadowTexture::activateDrawing(false);
}


void Scene::drawReflectionTexture() {
	if(GraphicsSettings::getReflections() == 0) {
		return;
	}
	setClearColor();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    ReflectionTexture::activateDrawing(true);

    Viewport viewport(0, 0,
    		          ReflectionTexture::getViewportWidth(),
    		          ReflectionTexture::getViewportHeight());
    _camera3D->setViewport(viewport);
	_camera3D->setLookAt(_player->getCameraLookAt());
	_camera3D->setFieldOfView(90.0);

    _camera3D->applyViewport();
    _camera3D->applyProjection();
    _camera3D->applyModelview();

	glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_FOG);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    _sun->apply();

	_fog->apply();

    glPopMatrix();

    _sun->draw();
	_sky->draw ( ) ;

    _map->draw(_player);

    _player->draw();

    _map->grabReflectionTexture();

    ReflectionTexture::activateDrawing(false);

}


void Scene::drawInterface()
{    
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
	if (false) { //TODO: good condition
		glPushMatrix();
		_dock->draw();	
		glPopMatrix();
	}	
	glPushMatrix();
    _menuManager->draw();
    glPopMatrix();
	
}



void Scene::loadIntroMap() {
	_gameSaveFunctions->loadIntroMap();
}


void Scene::loadGame(int gameSave) {
	_gameSaveFunctions->loadGame(gameSave);
}


void Scene::saveGame(int gameSave) {
	_gameSaveFunctions->saveGame(gameSave);
}


void Scene::onResize(int width, int height) {
    glutPostRedisplay();
}

void Scene::onKey(unsigned char key) {
    _inputManager->onKey(key);
}

void Scene::onKeyUp(unsigned char key) {
    _inputManager->onKeyUp(key);
}

void Scene::onSpecialKey(int key){
	_inputManager->onSpecialKey(key);
}

void Scene::onSpecialKeyUp(int key){
	_inputManager->onSpecialKeyUp(key);
}

void Scene::onMouseEntry(int state) {
}

void Scene::onMouseClick(int button, int state, int x, int y) {
	if(button == GLUT_MIDDLE_BUTTON || button == GLUT_RIGHT_BUTTON) {
		if(state == GLUT_DOWN) {
			glutSetCursor(GLUT_CURSOR_NONE);
		} else {
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		}
	}
	_inputManager->onMouseMove(x, y);
	_inputManager->onMouseClick(button, state);
}

void Scene::onMouseMove(int x, int y) {
	_inputManager->onMouseMove(x, y);
}

void Scene::onMousePassiveMove(int x, int y) {
	_inputManager->onMouseMove(x, y);
}

void Scene::onVisible(int state) {
    if(state == GLUT_VISIBLE) {
        glutPostRedisplay();
    }
}

void Scene::onTimer(int value) {
	float duration = _updateTimer.getMilliseconds() * 0.001;
	if(IntroScreen::isOn()) {
		_introScreen->update(duration);

	} else if(LoadingScreen::isOn()) {

	} else {
		_inputManager->update();
		cheat();
		_menuManager->update();
		_dock->update();
		if(!MenuManager::isMenuOpen()) {
			_player->update(duration);
			_npcManager->update(duration);
		}
		if(!MenuManager::isMenuOpen() || MenuManager::isInIntroMenuMode()) {
			_map->update(duration);
			_sky -> update ( duration ) ;
		}
	}
	glutPostRedisplay();
    _updateTimer.start();
}

void Scene::onIdle() {
}

Color3f Scene::getBackgroundColor() {
	return _backgroundColor;
}

bool Scene::isMapDebugOn() {
	return DEBUG_MAP;
}

void Scene::cheat() {
	if(InputManager::keyHeld('l') &&
	   InputManager::keyHeld('o') &&
	   InputManager::keyHeld('f')) {
		int cheatX, cheatY;
		std::cout << "Change player position:" << std::endl << "X: ";
		std::cin >> cheatX;
		std::cout << "Y: ";
		std::cin >> cheatY;
		_player->place(cheatX, cheatY, _player->getDirection());
		std::cout << "Placed player at: " << cheatX << "/" << cheatY << std::endl;
	}
}


GRAPHICSLAB_NAMESPACE_END



