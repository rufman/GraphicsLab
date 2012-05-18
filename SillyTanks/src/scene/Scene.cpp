/**
 * scene.cpp
 * This class creates the whole graphical scene with all the objects in it.
 */

// Class declaration include
#include "Scene.hpp"

//common includes
#include "../common/Exception.hpp"
#include "../common/Utils.hpp"

//windowing includes
#include "../windowing/Application.hpp"
#include "../windowing/Window.hpp"

//common includes
#include "../common/Camera2D.hpp"
#include "../common/Camera3D.hpp"
#include "../common/TextBox.hpp"
#include "../common/SoundEngine.hpp"

//scene includes
#include "SkyDome.hpp"
#include "Terrain.hpp"

//illumination includes
#include "illumination/DirectionalLight.hpp"
#include "illumination/PositionalLight.hpp"

//entities includes
#include "entities/Bullet.hpp"
#include "entities/Missile.hpp"
#include "entities/SmallTank.hpp"

//pathfinding includes
#include "pathfinding/Node.hpp"

#include <sstream>
#include <cmath>
#include <iostream>
#include <vector>

namespace game_space {

Scene::Scene( Window &window ):
_window( window ),
_gridDisplayList( 0 ),
_firstUpdate( true ),
_cameraMode( TANK_CAM ),
_overlayCam( NULL ),
_tankCam( NULL ),
_skyDome( NULL ),
_terrain( NULL ),
_sunLight(NULL),
_tank(NULL),
_missile(NULL)
{
	_soundEngine = SoundEngine();


	_endNode = new Node(Point(1,2,1), *this);
	_endNode->_pathState = Node::ENDPOINT;
}

Scene::~Scene() {
	delete _skyDome;
	delete _terrain;

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

	float ambientLight[4] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_NORMALIZE);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	Application &application = Application::getInstance();
	const Application::Parameters &parameters = application.getParameters();

	// Initialize scene components
	_overlayCam = new Camera2D(*this);
	_tankCam = new Camera3D(*this);
	_overviewCam = new Camera3D(*this);

	_skyDome = new SkyDome(*this, parameters.skyTextureFile, 500, 50, 50);
	_terrain = new Terrain(*this, parameters.terrainFilePrefix, 100 * 2,
			100 * 2, 50, 50);
	_tank = new SmallTank(*this);

	_tankSmokeParticleEngine = new ParticleEngine<Smoke>(_tankCam);
	_tankSmokeParticleEngine->setStartAcceleration(Vector3D(0, 0, 0));
	_tankSmokeParticleEngine->setActive(true);
	_missileSmokeParticleEngine = new ParticleEngine<Smoke>(_tankCam);
	_missileSmokeParticleEngine->setStartAcceleration(Vector3D(0, 0, 0));
	_missileSmokeParticleEngine->setActive(false);

	// Reset data
	reset();
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
	_tank->reset();
	for (std::vector<Bullet*>::iterator bulletIter = _bullets.begin();
			bulletIter != _bullets.end(); ++bulletIter) {
		Bullet *bullet = *bulletIter;
		delete bullet;
	}
	_bullets.clear();

	_missile = NULL;
}

void Scene::update(float seconds) {
	handleKeyboardInput();

	_skyDome->update(seconds);
	_tank->update(seconds);

	for (std::vector<Bullet*>::iterator bulletIter = _bullets.begin();
			bulletIter != _bullets.end();) {
		Bullet *bullet = *bulletIter;
		bullet->move(seconds);

		bool hitTarget = false;

		//implement here how to checkhits between targets and bullets
		/*for ( TargetVector::iterator targetIter = _targets.begin();targetIter != _targets.end();++targetIter )
		 {
		 Target *target = *targetIter;

		 if ( target->checkHit( *bullet ) )
		 {
		 target->setHit( true );
		 hitTarget = true;
		 break;
		 }
		 }*/

		// Remove bullet if out of window
		const Point &bulletPosition = bullet->getPosition();

		if (hitTarget
				|| (bulletPosition.y < _terrain->getHeight(bulletPosition))) {
			bulletIter = _bullets.erase(bulletIter);
			_terrain->doDamageAt(bulletPosition);
			delete bullet;
		} else {
			++bulletIter;
		}
	}

	if (_missile != NULL) {
		_missile->move(seconds);
		_missileSmokeParticleEngine->setStartPosition(_missile->getPosition());
		const Point &missilePosition = _missile->getPosition();
		if (missilePosition.y < _terrain->getHeight(missilePosition)) {
			_terrain->doDamageAt(missilePosition);
			_terrain->doDamageAt(missilePosition);
			_terrain->doDamageAt(missilePosition);
			_missile->~Missile();
			_missile = NULL;
			_missileSmokeParticleEngine->setActive(false);
		}
	}
	_missileSmokeParticleEngine->update(seconds);

	_tankSmokeParticleEngine->setStartPosition(_tank->getPosition());
	_tankSmokeParticleEngine->update(seconds);
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

	// Clear the screen
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

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
	Camera3D *camera;

	if (_cameraMode == TANK_CAM) {

		camera = _tankCam;
		camera->setLookAt(_tank->getLookAt());
	} else if (_cameraMode == OVERVIEW_CAM) {
		camera = _overviewCam;
		camera->setLookAt(
				LookAt(Point(100, 100, 100), _tank->getPosition(),
						Vector3D(0, 1, 0)));
	}

	// OpenGL Lighting
	camera->setViewport(Viewport(0, 0, width, height));
	camera->applyViewport();
	camera->applyProjection();
	camera->applyModelview();

	// Grid
	if (_gridOn)
		drawGrid();

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
	glPushMatrix();

	// Draw the sky
	_skyDome->setRenderingParameters(_renderingParameters);
	_skyDome->draw();

	// Draw the terrain
	_terrain->setRenderingParameters(_renderingParameters);
	_terrain->draw();

	//Draw the tank
	_tank->setRenderingParameters(_renderingParameters);
	_tank->draw();

	glPopMatrix();

	glPushMatrix();

	// Draw bullets
	for (std::vector<Bullet*>::iterator bulletIter = _bullets.begin();
			bulletIter != _bullets.end(); ++bulletIter) {
		Bullet *bullet = *bulletIter;
		bullet->setRenderingParameters(_renderingParameters);
		bullet->draw();
	}

	glPopMatrix();

	glPushMatrix();
	if (_missile != NULL) {
		_missile->draw();
	}
	glPopMatrix();

	glPushMatrix();
	_tankSmokeParticleEngine->draw();
	glPopMatrix();

	glPushMatrix();
	_missileSmokeParticleEngine->draw();
	glPopMatrix();
	_endNode->draw();
}

void Scene::drawGrid() {
	GLboolean lightingEnabled;
	glGetBooleanv(GL_LIGHTING, &lightingEnabled);
	if (lightingEnabled) {
		glDisable(GL_LIGHTING);
	}

	float lineWidth;
	glGetFloatv(GL_LINE_WIDTH, &lineWidth);

	float width = 100.0;

	if (_gridDisplayList) {
		glCallList(_gridDisplayList);
	} else {
		_gridDisplayList = glGenLists(1);

		glNewList(_gridDisplayList, GL_COMPILE_AND_EXECUTE);

		glLineWidth(2.0f);
		glBegin(GL_LINES);

		glColor3fv(Color::RED());
		glVertex3f(-width, 0.0, 0.0);
		glVertex3f(width, 0.0, 0.0);
		glColor3fv(Color::GREEN());
		glVertex3f(0.0, -width, 0.0);
		glVertex3f(0.0, width, 0.0);
		glColor3fv(Color::BLUE());
		glVertex3f(0.0, 0.0, -width);
		glVertex3f(0.0, 0.0, width);

		glEnd();

		uint resolution = 10;
		float tickWidth = 0.12;
		glLineWidth(0.5f);
		glBegin(GL_LINES);

		glColor3fv(Color::RED());
		for (uint i = 0; i < resolution; i++) {
			float x = width - (2 * width) * i * 1.0 / resolution;
			glVertex3f(x, 0.0, -tickWidth);
			glVertex3f(x, 0.0, tickWidth);
			glVertex3f(x, -tickWidth, 0.0);
			glVertex3f(x, tickWidth, 0.0);
		}
		glColor3fv(Color::GREEN());
		for (uint i = 0; i < resolution; i++) {
			float y = width - (2 * width) * i * 1.0 / resolution;
			glVertex3f(-tickWidth, y, 0.0);
			glVertex3f(tickWidth, y, 0.0);
			glVertex3f(0.0, y, -tickWidth);
			glVertex3f(0.0, y, tickWidth);
		}
		glColor3fv(Color::BLUE());
		for (uint i = 0; i < resolution; i++) {
			float z = width - (2 * width) * i * 1.0 / resolution;
			glVertex3f(-tickWidth, 0.0, z);
			glVertex3f(tickWidth, 0.0, z);
			glVertex3f(0.0, -tickWidth, z);
			glVertex3f(0.0, tickWidth, z);
		}

		glEnd();

		float textHeight = 1.0 / width;
		TextBox x(*this, "x", textHeight, 1.5);
		TextBox y(*this, "y", textHeight, 1.5);
		TextBox z(*this, "z", textHeight, 1.5);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(width * 1.02, 0.0, x.getWidth() / 2.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		glColor3fv(Color::RED());
		x.draw();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-y.getWidth() / 2.0, width * 1.02, 0.0);
		glColor3fv(Color::GREEN());
		y.draw();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-z.getWidth() / 2.0, 0.0, width * 1.02);
		glColor3fv(Color::BLUE());
		z.draw();
		glPopMatrix();

		glEndList();
	}

	if (lightingEnabled)
		glEnable(GL_LIGHTING);

	glLineWidth(lineWidth);
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

void Scene::fireBullet() {
	Bullet *bullet = new Bullet(*this);

	_soundEngine.playMuzzleSound();

	bullet->setPosition(_tank->getMuzzlePosition());

	float velocityScale = 30;
	Vector3D velocity(
			-velocityScale * _tank->getShootingPower()
					* std::cos(Utils::toRadian(_tank->getElevation()))
					* std::sin(Utils::toRadian(-_tank->getAzimuth())),
			velocityScale * _tank->getShootingPower()
					* std::sin(Utils::toRadian(_tank->getElevation())),
			-velocityScale * _tank->getShootingPower()
					* std::cos(Utils::toRadian(_tank->getElevation()))
					* std::cos(Utils::toRadian(-_tank->getAzimuth())));
	bullet->setVelocity(velocity);

	_bullets.push_back(bullet);
}

void Scene::fireMissile() {
	if (_missile == NULL) {
		_missile = new Missile(*this);

		_missile->setPosition(_tank->getMuzzlePosition());

		_missileSmokeParticleEngine->setActive(true);
		_missileSmokeParticleEngine->setStartPosition(_missile->getPosition());

		float velocityScale = 10;
		Vector3D velocity(
				-velocityScale * _tank->getShootingPower()
						* std::cos(Utils::toRadian(_tank->getElevation()))
						* std::sin(Utils::toRadian(-_tank->getAzimuth())),
				velocityScale * _tank->getShootingPower()
						* std::sin(Utils::toRadian(_tank->getElevation())),
				-velocityScale * _tank->getShootingPower()
						* std::cos(Utils::toRadian(_tank->getElevation()))
						* std::cos(Utils::toRadian(-_tank->getAzimuth())));
		_missile->setVelocity(velocity);

		_soundEngine.playExplosionSound();

	}
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
		_tank->move(SMALLTANK_SPEED);
	}

	if (_window.keyPressed('a') || _window.keyPressed('A')) {
		_tank->setDirection(_tank->getDirection() - 5);
	}

	if (_window.keyPressed('s') || _window.keyPressed('S')) {
		_tank->move(-SMALLTANK_SPEED);
	}

	if (_window.keyPressed('d') || _window.keyPressed('D')) {
		_tank->setDirection(_tank->getDirection() + 5);
	}

	if (_window.keyPressed('r') || _window.keyPressed('R')) {
		reset();
		glutPostRedisplay();
	}

	if (_window.keyPressed('+')) {
		float power = _tank->getShootingPower() + 0.1;
		_tank->setShootingPower((power > 1.0) ? 1.0 : power);
	}

	if (_window.keyPressed('-')) {
		float power = _tank->getShootingPower() - 0.1;
		_tank->setShootingPower((power < 0.1) ? 0.1 : power);
	}

	if (_window.keyPressed(' ')) {
		fireBullet();
	}

	if (_window.keyPressed('m') || _window.keyPressed('M')) {
		fireMissile();
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

	if (_window.keyHit('3')) {
		drawGrid();
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
	if (_window.keyHit('p') || _window.keyHit('P')) {
		_terrain->findPath(_tank->getPosition(), _endNode->_position);
	}
	if (_window.specialKeyPressed(GLUT_KEY_LEFT)) {
		_endNode->_position.x--;
		//_endNode->_position.y = getTerrain().getHeight(_endNode->_position);
	}
	if (_window.specialKeyPressed(GLUT_KEY_RIGHT)) {
		_endNode->_position.x++;
		//_endNode->_position.y = getTerrain().getHeight(_endNode->_position);
	}
	if (_window.specialKeyPressed(GLUT_KEY_UP)) {
		_endNode->_position.z--;
		//_endNode->_position.y = getTerrain().getHeight(_endNode->_position);
	}
	if (_window.specialKeyPressed(GLUT_KEY_DOWN)) {
		_endNode->_position.z++;
		//_endNode->_position.y = getTerrain().getHeight(_endNode->_position);
	}
}

void Scene::onMouseEntry(int state) {
	if (state == GLUT_ENTERED) {
	} else if (state == GLUT_LEFT) {
	}
}

void Scene::onMouseClick(int button, int state, int x, int y) {

	fireBullet();
}

void Scene::onMouseMove(int x, int y) {
	//TODO:onMouseMove does not work as expected
	fireBullet();
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	int xMove = x - _mouseX;
	int yMove = y - _mouseY;
	_mouseX = width / 2;
	_mouseY = height / 2;

	if (abs(xMove) > 15 || abs(yMove) > 15) {
		glutWarpPointer(width / 2, height / 2);
	}

	_tank->setElevation(_tank->getElevation() - yMove / 3);
	_tank->setAzimuth(_tank->getAzimuth() + xMove / 3);
	glutPostRedisplay();
}

void Scene::onMousePassiveMove(int x, int y) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	int xMove = x - _mouseX;
	int yMove = y - _mouseY;
	_mouseX = width / 2;
	_mouseY = height / 2;
	if (abs(xMove) > 15 || abs(yMove) > 15) {
		glutWarpPointer(width / 2, height / 2);
	}

	_tank->setElevation(_tank->getElevation() - yMove / 7);
	_tank->setAzimuth(_tank->getAzimuth() + xMove / 7);
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

}
