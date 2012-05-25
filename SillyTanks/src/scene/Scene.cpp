/**
 * scene.cpp
 * This class creates the whole graphical scene with all the objects in it.
 */

// Class declaration include
#include "Scene.hpp"

//common includes
#include "../common/Exception.hpp"
#include "../common/Utils.hpp"
#include "../common/Camera2D.hpp"
#include "../common/Camera3D.hpp"
#include "../common/TextBox.hpp"
#include "../common/SoundEngine.hpp"
#include "../common/PLYModel.hpp"

//windowing includes
#include "../windowing/Application.hpp"
#include "../windowing/Window.hpp"

//scene includes
#include "SkyDome.hpp"
#include "Terrain.hpp"
#include "Water.hpp"
#include "Mirror.hpp"
#include "Fog.hpp"

//illumination includes
#include "illumination/DirectionalLight.hpp"
#include "illumination/PositionalLight.hpp"

//entities includes
#include "entities/targets/SmallTank.hpp"
#include "entities/targets/SmallTower.hpp"

#include "AI/TankAI.hpp"
#include "AI/TowerAI.hpp"
#include "AI/MessageBus.hpp"

//targets includes
#include "entities/targets/PineTree.hpp"

//pathfinding includes
#include "pathfinding/Node.hpp"

//particle engine includes
#include "particleEngine/ParticleEngine.hpp"
#include "particleEngine/Smoke.hpp"

//collision detection includes
#include "entities/collisiondetection/Projectile.hpp"

//projectile includes
#include "entities/projectiles/Missile.hpp"
#include "entities/projectiles/Bullet.hpp"

#include "illumination/shading/ShadingEngine.hpp"

//std includes
#include <sstream>
#include <cmath>
#include <iostream>
#include <vector>

namespace game_space {

typedef float GLmatrix16f[16]; // Typedef's For VMatMult Procedur

void VMatMult(GLmatrix16f M, Point v) {
	GLfloat res[4]; // Hold Calculated Results
	res[0] = M[0] * v.x + M[4] * v.y + M[8] * v.z + M[12] * v.w;
	res[1] = M[1] * v.x + M[5] * v.y + M[9] * v.z + M[13] * v.w;
	res[2] = M[2] * v.x + M[6] * v.y + M[10] * v.z + M[14] * v.w;
	res[3] = M[3] * v.x + M[7] * v.y + M[11] * v.z + M[15] * v.w;
	v.x = res[0]; // Results Are Stored Back In v
	v.y = res[1];
	v.z = res[2];
	v.w = res[3]; // Homogenous Coordinate
}

Scene::Scene(Window &window) :
		_window(window), _gridDisplayList(0), _firstUpdate(true), _cameraMode(
				TANK_CAM), _overlayCam(NULL), _tankCam(NULL), _skyDome(NULL), _terrain(
				NULL), _water(NULL), _sunLight(NULL), _shadowsActive(false), _fogActive(
				false), _shadersAlreadyCompiled(false), _shaderActive(false) {
	_soundEngine = SoundEngine();
	_messageBus = new MessageBus();

	_endNode = new Node(Point(1, 2, 1), *this);
	_endNode->_pathState = Node::ENDPOINT;
}

Scene::~Scene() {
	delete _skyDome;
	delete _terrain;
	delete _water;

	for (LightVector::iterator lightIter = _lights.begin();
			lightIter != _lights.end(); ++lightIter) {
		Light *light = *lightIter;
		delete light;
	}
	_lights.clear();

	delete _overlayCam;
	delete _tankCam;
	delete _overviewCam;

	glDeleteLists(_gridDisplayList, 1);
}

void Scene::initialize() {

	// Initialize lights
	Color lightAmbient(0.4, 0.4, 0.4);
	Color lightSpecular(0.8, 0.8, 0.8);
	Color lightDiffuse(0.8, 0.8, 0.8);

	Light *light = new DirectionalLight(*this);
	light->setAmbient(lightAmbient);
	light->setDiffuse(lightDiffuse);
	light->setSpecular(lightSpecular);
	light->setPosition(Point(0.0, 50, 0.0));
	light->turnOn(true);
	_lights.push_back(light);

	_sunLight = new DirectionalLight(*this);
	_sunLight->setAmbient(lightAmbient);
	_sunLight->setDiffuse(lightDiffuse);
	_sunLight->setSpecular(lightSpecular);
	_sunLight->setPosition(Point(0.0, 50, 0.0));
	_sunLight->turnOn(true);

	// Anti-aliasing
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	//ambient light of the scene
	float ambientLight[4] = { 0.5, 0.5, 0.5, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_NORMALIZE);

	Application &application = Application::getInstance();
	const Application::Parameters &parameters = application.getParameters();

	glClearColor(parameters.fogRed, parameters.fogGreen, parameters.fogBlue,
			0.0);

	// Initialize scene components
	_overlayCam = new Camera2D(*this);
	_tankCam = new Camera3D(*this);
	_overviewCam = new Camera3D(*this);
	_currentlyActiveCamera = _tankCam;

	_skyDome = new SkyDome(*this, parameters.skyTextureFile, 500, 50, 50);
	_terrain = new Terrain(*this, parameters.terrainFilePrefix, 100 * 4,
			100 * 4, 50, 50);
	_water = new Water(*this, parameters.waterHeight, 100 * 4, 100 * 4);

	//add playertank to game
	_playerTank = new SmallTank(*this, false);

	_playerTank->setPosition(_terrain->getRandomPointOnMap());
	_playerTank->setNeighbors();
	_targets.push_back(_playerTank);

	//add some tanks to the scene
	for (int i = 0; i < 10; i++) {
		Tank* tank = new SmallTank(*this, true);
		tank->setPosition(_terrain->getRandomPointOnMap());
		_targets.push_back(tank);
	}

	//add some towers to the scene
	for (int i = 0; i < 4; i++) {
		Tower* tower = new SmallTower(*this, true);
		tower->setPosition(_terrain->getRandomPointOnMap());
		_targets.push_back(tower);
	}

	_fog = new Fog(parameters.fogDensity, parameters.fogStart,
			parameters.fogEnd, parameters.fogRed, parameters.fogGreen,
			parameters.fogBlue);
	// reset data
	reset();

	//hiding the default cursor and putting the cursor position to the middle of the window
	glutSetCursor(GLUT_CURSOR_NONE);

	//move the pointer to the middle of the panel
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2,
			glutGet(GLUT_WINDOW_HEIGHT) / 2);

}

void Scene::reset() {
	_renderingParameters.drawMode = RenderingParameters::POLYGON;
	_renderingParameters.shadeMode = RenderingParameters::SMOOTH;

	_dashBoardOn = false;
	_gridOn = false;

	_firstUpdate = true;

	_cameraMode = TANK_CAM;
	_freeCameraParameters.reset();
	_freeCameraParameters.applyToCamera(*_tankCam);

	_skyDome->reset();
	_water->reset();

	for (std::vector<Projectile*>::iterator projectileIter =
			_projectiles.begin(); projectileIter != _projectiles.end();) {
		Projectile *projectile = *projectileIter;
		if (projectile->_projectileType == Projectile::BULLET) {
			Bullet* bullet = static_cast<Bullet*>(projectile);
			projectileIter = _projectiles.erase(projectileIter);
			delete bullet;
		}
		if (projectile->_projectileType == Projectile::MISSILE) {
			Missile* missile = static_cast<Missile*>(projectile);

			projectileIter = _projectiles.erase(projectileIter);
			delete missile;
		}
	}
}

void Scene::update(float seconds) {

	if (_shaderActive && !_shadersAlreadyCompiled) {
		_shadingEngine = ShadingEngine();
		_shadersAlreadyCompiled = true;
	}

	handleKeyboardInput();

	_skyDome->update(seconds);
	_water->update(seconds);

	for (std::vector<Target*>::iterator targetIter = _targets.begin();
			targetIter != _targets.end(); targetIter++) {
		Target* target = *targetIter;
		if (target->_targetType == Target::TANK) {
			Tank* tank = static_cast<Tank*>(target);
			if (tank->isAIControlled()) {
				tank->getAI()->brainTick(seconds);
			}
			tank->update(seconds);
		}
		else if (target->_targetType == Target::TOWER) {
			Tower * tower = static_cast<Tower*>(target);
			if (tower->isAIControlled()) {
				tower->getAI()->brainTick(seconds);
			}
			tower->update(seconds);
		}
	}

	for (std::vector<Projectile*>::iterator projectileIter =
			_projectiles.begin(); projectileIter != _projectiles.end();) {
		Projectile *projectile = *projectileIter;
		if (projectile->_projectileType == Projectile::BULLET) {
			Bullet* bullet = static_cast<Bullet*>(projectile);
			bullet->move(seconds);

			if (bullet->isDetonated()) {
				projectileIter = _projectiles.erase(projectileIter);
				delete bullet;
			} else {
				++projectileIter;
			}
		}
		else if (projectile->_projectileType == Projectile::MISSILE) {
			Missile* missile = static_cast<Missile*>(projectile);
			missile->move(seconds);
			if (missile->isDetonated()) {
				projectileIter = _projectiles.erase(projectileIter);
				delete missile;
			} else {
				++projectileIter;
			}
		}
	}
}

void Scene::onPaint() {
	if (_firstUpdate) {
		_updateTime.start();
		_firstUpdate = false;
	} else {
		float secondsSinceLastUpdate = _updateTime.getMilliseconds() / 1000.0;
		update(secondsSinceLastUpdate);
		_updateTime.start();
	}
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
// Draw mirror image
	drawWaterImage();
	_water->capture();
// Clear the screen
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

// Draw screen
	drawScene();
	if (_dashBoardOn)
		drawOverlay();

// Swap buffers
	glutSwapBuffers();
}

void Scene::drawScene() {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

// Set camera parameters

	if (_cameraMode == TANK_CAM) {
		_currentlyActiveCamera = _tankCam;
		_currentlyActiveCamera->setLookAt(_playerTank->getLookAt());
	} else if (_cameraMode == OVERVIEW_CAM) {
		_currentlyActiveCamera = _overviewCam;
		_currentlyActiveCamera->setLookAt(
				LookAt(
						Point(_playerTank->getPosition().x + 50,
								_playerTank->getPosition().y + 50,
								_playerTank->getPosition().z + 50),
						_playerTank->getPosition(), Vector3D(0, 1, 0)));
	}

// OpenGL Lighting
	_currentlyActiveCamera->setViewport(Viewport(0, 0, width, height));
	_currentlyActiveCamera->applyViewport();
	_currentlyActiveCamera->applyProjection();
	_currentlyActiveCamera->applyModelview();


// Set scene parameters
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

	for (LightVector::iterator lightIter = _lights.begin();
			lightIter != _lights.end(); ++lightIter) {
		Light *light = *lightIter;
		light->apply();
	}

// draw sun light
	_sunLight->setPosition(_skyDome->getSunPosition());
	_sunLight->apply();

// Draw scene
	glMatrixMode(GL_MODELVIEW);

//The Sky dome should not use toon shading
	if (_shaderActive) {
		_shadingEngine.clearShaders();
	}
	glPushMatrix();

// Draw the sky
	_skyDome->setRenderingParameters(_renderingParameters);
	_skyDome->draw();

//draw water
	_water->setRenderingParameters(_renderingParameters);
	_water->draw();

//the terrain should use toon shading
	if (_shaderActive) {
		_shadingEngine.applyToonShader();
	}

// Draw the terrain
	_terrain->setRenderingParameters(_renderingParameters);
	_terrain->draw();

//Draw the targets
	for (std::vector<Target*>::iterator targetIter = _targets.begin();
			targetIter != _targets.end(); targetIter++) {
		Target* target = *targetIter;
		if (target->_targetType == Target::TANK) {
			Tank* tank = static_cast<Tank*>(target);
			tank->draw();
		}
		if (target->_targetType == Target::TOWER) {
			Tower* tower = static_cast<Tower*>(target);
			tower->draw();
		}
	}
//_playerTank->drawShadow(lightpos);
	glPopMatrix();

//the bullets should not use toon shading
	if (_shaderActive) {
		_shadingEngine.clearShaders();
	}
	glPushMatrix();
	for (std::vector<Projectile*>::iterator projectileIter =
			_projectiles.begin(); projectileIter != _projectiles.end();
			++projectileIter) {
		Projectile *projectile = *projectileIter;
		if (projectile->_projectileType == Projectile::BULLET) {
			Bullet* bullet = static_cast<Bullet*>(projectile);
			bullet->setRenderingParameters(_renderingParameters);
			bullet->draw();
		}
		if (projectile->_projectileType == Projectile::MISSILE) {
			Missile* missile = static_cast<Missile*>(projectile);
			missile->setRenderingParameters(_renderingParameters);
			missile->draw();
		}
	}

	glPopMatrix();

	_endNode->draw();

//if shadows are active then draw shadows
	if (_shadowsActive) {
		GLmatrix16f Minv;
		Point lightpos = Point(100, 100, 100);
		//Point lightpos = _skyDome->getSunPosition();
		glClearDepth(1.0f); // Depth Buffer Setup
		glClearStencil(0); // Stencil Buffer Setup
		glEnable(GL_DEPTH_TEST); // Enables Depth Testing
		glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
		glCullFace(GL_BACK); // Set Culling Face To Back Face
		glEnable(GL_CULL_FACE); // Enable Culling

		//glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, Minv); // Retrieve ModelView Matrix (Stores In Minv)
		VMatMult(Minv, lightpos); // We Store Rotated Light Vector In 'lp' Array
		glGetFloatv(GL_MODELVIEW_MATRIX, Minv); // Retrieve ModelView Matrix From Minv
		Point wlp = Point(0, 0, 0, 1);
		lightpos.x += wlp.x;
		lightpos.y += wlp.y;
		lightpos.z += wlp.z;
		lightpos.w += wlp.w;

		_terrain->drawShadows(lightpos);
	}

	glEnable(GL_LIGHTING); // Enable Lighting
	glDepthMask(GL_TRUE); // Enable Depth Mask

	if (_cameraMode == TANK_CAM) {
		if (_fogActive) {
			_fog->apply();
		} else {
			_fog->remove();
		}
	} else if (_cameraMode == OVERVIEW_CAM) {
		_fog->remove();
	}
	glFlush();
	glutSwapBuffers();
}

void Scene::drawOverlay() {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT) / 4;

// Set camera parameters
	_overlayCam->setViewport(
			Viewport(0, glutGet(GLUT_WINDOW_HEIGHT) - height, width, height));

	_overlayCam->applyViewport();
	_overlayCam->applyProjection();
	_overlayCam->applyModelview();

// Set overlay parameters
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

// Draw overlay
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glColor4f(0.6, 0.6, 0.0, 0.4);
	glRectd(0, 0, width, height);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glEnd();

	glPopMatrix();
}

void Scene::onResize(int width, int height) {
	glutPostRedisplay();
}

void Scene::handleKeyboardInput() {
	/*int modifier = glutGetModifiers();
	 bool shiftPressed = ( modifier & GLUT_ACTIVE_SHIFT );
	 bool ctrlPressed = ( modifier & GLUT_ACTIVE_CTRL );
	 bool altPressed = ( modifier & GLUT_ACTIVE_ALT );*/

	if (_window.keyPressed('w') || _window.keyPressed('W')) {
		_playerTank->move(SMALLTANK_SPEED);
	}

	if (_window.keyPressed('a') || _window.keyPressed('A')) {
		_playerTank->setDirection(_playerTank->getDirection() - 5);
	}

	if (_window.keyPressed('s') || _window.keyPressed('S')) {
		_playerTank->move(-SMALLTANK_SPEED);
	}

	if (_window.keyPressed('d') || _window.keyPressed('D')) {
		_playerTank->setDirection(_playerTank->getDirection() + 5);
	}

	if (_window.keyPressed('r') || _window.keyPressed('R')) {
		reset();
		glutPostRedisplay();
	}

	if (_window.keyPressed('+')) {
		float power = _playerTank->getShootingPower() + 0.1;
		_playerTank->setShootingPower((power > 1.0) ? 1.0 : power);
	}

	if (_window.keyPressed('-')) {
		float power = _playerTank->getShootingPower() - 0.1;
		_playerTank->setShootingPower((power < 0.1) ? 0.1 : power);
	}

	if (_window.keyHit('1')) {
		if (_renderingParameters.drawMode == RenderingParameters::WIREFRAME) {
			_renderingParameters.drawMode = RenderingParameters::POLYGON;
		} else {
			_renderingParameters.drawMode = RenderingParameters::WIREFRAME;
		}
	}

	if (_window.keyHit('2')) {
		if (_renderingParameters.shadeMode == RenderingParameters::SMOOTH) {
			_renderingParameters.shadeMode = RenderingParameters::FLAT;
		} else {
			_renderingParameters.shadeMode = RenderingParameters::SMOOTH;
		}
	}

	if (_window.keyHit('4')) {
		if (_renderingParameters.normalMode == RenderingParameters::OFF) {
			_renderingParameters.normalMode = RenderingParameters::VERTEX;
		} else if (_renderingParameters.normalMode
				== RenderingParameters::VERTEX) {
			_renderingParameters.normalMode = RenderingParameters::TRIANGLE;
		} else {
			_renderingParameters.normalMode = RenderingParameters::OFF;
		}
	}
	if (_window.keyHit('5')) {
		if (_cameraMode == TANK_CAM) {
			_cameraMode = OVERVIEW_CAM;
		} else if (_cameraMode == OVERVIEW_CAM) {
			_cameraMode = TANK_CAM;
		}
	}
	if (_window.keyHit('6')) {
		_shadowsActive = !_shadowsActive;
	}
	if (_window.keyHit('7')) {
		_fogActive = !_fogActive;
	}
	if (_window.keyHit('8')) {
		_shaderActive = !_shaderActive;
	}
	if (_window.keyHit('p') || _window.keyHit('P')) {
		_terrain->findPath(_playerTank->getPosition(), _endNode->_position);
	}
	if (_window.specialKeyPressed(GLUT_KEY_LEFT)) {
		_endNode->_position.x--;
		_endNode->_position.y = getTerrain().getHeight(_endNode->_position);
	}
	if (_window.specialKeyPressed(GLUT_KEY_RIGHT)) {
		_endNode->_position.x++;
		_endNode->_position.y = getTerrain().getHeight(_endNode->_position);
	}
	if (_window.specialKeyPressed(GLUT_KEY_UP)) {
		_endNode->_position.z--;
		_endNode->_position.y = getTerrain().getHeight(_endNode->_position);
	}
	if (_window.specialKeyPressed(GLUT_KEY_DOWN)) {
		_endNode->_position.z++;
		_endNode->_position.y = getTerrain().getHeight(_endNode->_position);
	}
}

void Scene::onMouseEntry(int state) {
	if (state == GLUT_ENTERED) {
	} else if (state == GLUT_LEFT) {
	}
}

void Scene::onMouseClick(int button, int state, int x, int y) {

// do not change these as they are defined like that from the mouse api of opengl, I just did not find the constants
	const int leftButton = 0;
	const int middleButton = 1;
	const int rightButton = 2;
	const int mouseDown = 0;
	const int mouseUp = 1;

	if (state == mouseDown) {
		switch (button) {
		case leftButton: {
			switch (_playerTank->getSelectedWeapon()) {
			case Tank::BULLET: {
				_playerTank->fireBullet();
				break;
			}
			case Tank::MISSILE: {
				_playerTank->fireMissile(_terrain->getRandomPointOnMap());
				break;
			}
			}

			break;
		}
		case rightButton: {
			switch (_playerTank->getSelectedWeapon()) {
			case Tank::BULLET: {
				_playerTank->setSelectedWeapon(Tank::MISSILE);
				break;
			}
			case Tank::MISSILE: {
				_playerTank->setSelectedWeapon(Tank::BULLET);
				break;
			}
			}
			break;
		}
		}
	}
}

void Scene::onMouseMove(int x, int y) {
	onMousePassiveMove(x, y);
}

void Scene::onMousePassiveMove(int x, int y) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	int xMove = x - _mouseX;
	int yMove = y - _mouseY;
	_mouseX = width / 2;
	_mouseY = height / 2;
	if (abs(xMove) > 3 || abs(yMove) > 3) {
		glutWarpPointer(width / 2, height / 2);
	}

	_playerTank->setElevation(_playerTank->getElevation() - yMove / 2);
	_playerTank->setAzimuth(_playerTank->getAzimuth() + xMove / 2);
	glutPostRedisplay();
}

void Scene::onVisible(int state) {
	if (state == GLUT_VISIBLE)
		glutPostRedisplay();
}

void Scene::onTimer(int value) {
	glutPostRedisplay();
}

void Scene::onIdle() {
}

void Scene::FreeCameraParameters::applyToCamera(Camera3D &camera) {
	Point from(
			radius * std::cos(Utils::toRadian(elevation))
					* std::sin(Utils::toRadian(azimuth)) * -1,
			radius * std::sin(Utils::toRadian(elevation)),
			radius * std::cos(Utils::toRadian(elevation))
					* std::cos(Utils::toRadian(azimuth)) * -1);

	Vector3D up(0.0, 1.0, 0.0);
	Vector3D dir(-from.x, -from.y, -from.z);
	Vector3D normal = Utils::cross(up, dir);

	Vector3D normalMove = normal;
	normalMove *= moveX;
	Vector3D upMove = up;
	upMove *= moveY;

	Point newFrom(from.x + normalMove.x + upMove.x,
			from.y + normalMove.y + upMove.y, from.z + normalMove.z + upMove.z);
	Point to(newFrom.x - from.x, newFrom.y - from.y, newFrom.z - from.z);

	camera.setLookAt(LookAt(newFrom, to, up));
}

Window& Scene::getWindow() {
	return _window;
}

SoundEngine Scene::getSoundEngine() {
	return _soundEngine;
}

ShadingEngine Scene::getShadingEngine() {
	return _shadingEngine;
}

void Scene::setPlayerTank(Tank* tank) {
	_playerTank = tank;
}

Tank* Scene::getPlayerTank() {
	return _playerTank;
}
Camera3D* Scene::getTankCam() {
	return _tankCam;
}

Camera3D* Scene::getCurrentlyActiveCamera() {
	return _currentlyActiveCamera;
}

MessageBus* Scene::getMessageBus() {
	return _messageBus;
}

/**
 * Water mirroring part
 */

void Scene::drawWaterImage() {

// Set camera parameters
	_water->applyCamera();

// Set scene parameters
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);


	for (LightVector::iterator lightIter = _lights.begin();
			lightIter != _lights.end(); ++lightIter) {
		Light *light = *lightIter;
		light->apply();
	}

// Draw scene
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	_skyDome->draw();
	glPopMatrix();
}

Water* Scene::getWater() {
	return _water;
}

std::vector<Target*> Scene::getTargets() {
	return _targets;
}
}
