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
#include "entities/projectiles/Robot.hpp"

#include "illumination/shading/ShadingEngine.hpp"

#include "AI/DetonationSoundMessage.hpp"
#include "AI/AttackedByMessage.hpp"

//std includes
#include <sstream>
#include <cmath>
#include <iostream>
#include <vector>

namespace game_space {

typedef float GLmatrix16f[16]; // Typedef's For VMatMult Procedure

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
		_window(window), _firstUpdate(true), _cameraMode(TANK_CAM), _overlayCam(NULL), _tankCam(NULL), _overviewCam(NULL), _shadowsActive(false), _fogActive(false), _shaderActive(false), _soundActive(false), _shadersAlreadyCompiled(false), _chooseTarget(false), _sunLight(NULL), _skyDome(NULL), _terrain(NULL), _water(NULL), _fog(NULL), _targetChooser(Point(0, 0, 0)), _hudClockFace(NULL), _hudBullet(
				NULL), _hudMissile(NULL), _hudRobot(NULL), _hudStatusBars(NULL) {

	//create the soundengine
	_soundEngine = SoundEngine();
	_soundEngine.setActive(false);

	//create the messagebus
	_messageBus = new MessageBus();

	//i can not remove it because the game crashes...strange bug...
	_endNode = new Node(Point(1, 2, 1), *this);

	glClearColor(0, 0, 0, 0);

}

Scene::~Scene() {
	delete _skyDome;
	delete _terrain;
	delete _water;
	delete _fog;

	// delete lights
	for (LightVector::iterator lightIter = _lights.begin(); lightIter != _lights.end(); ++lightIter) {
		Light *light = *lightIter;
		delete light;
	}
	_lights.clear();

	// delete cameras
	delete _overlayCam;
	delete _tankCam;
	delete _overviewCam;

	delete _endNode;

	//delete targets
	delete _playerTank;
	for (std::vector<Target*>::iterator targetIter = _targets.begin(); targetIter != _targets.end(); targetIter++) {
		Target* target = *targetIter;
		delete target;
	}

	//delete projectiles
	for (std::vector<Projectile*>::iterator projectileIter = _projectiles.begin(); projectileIter != _projectiles.end();) {
		Projectile* projectile = *projectileIter;
		delete projectile;
	}

	delete _hudBullet;
	delete _hudMissile;
	delete _hudRobot;
	delete _hudStatusBars;
}

void Scene::initialize() {

	// Initialize lights
	Color lightAmbient(1.0, 1.0, 1.0);
	Color lightSpecular(1.0, 1.0, 1.0);
	Color lightDiffuse(1.0, 1.0, 1.0);

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
	float ambientLight[4] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_NORMALIZE);

	//hiding the default cursor and putting the cursor position to the middle of the window
	glutSetCursor(GLUT_CURSOR_NONE);

	//move the pointer to the middle of the panel
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

	Application &application = Application::getInstance();
	const Application::Parameters &parameters = application.getParameters();

	_fog = new Fog(parameters.fogDensity, parameters.fogStart, parameters.fogEnd, parameters.fogRed, parameters.fogGreen, parameters.fogBlue);

	// Initialize cameras
	_overlayCam = new Camera2D(*this);
	_tankCam = new Camera3D(*this);
	_overviewCam = new Camera3D(*this);
	_currentlyActiveCamera = _tankCam;

	//##############################
	//initialize scene components
	_skyDome = new SkyDome(*this, parameters.skyTextureFile, 500, 50, 50);
	_terrain = new Terrain(*this, parameters.terrainFilePrefix, 150 * 4, 150 * 4, 50, 50);
	_water = new Water(*this, parameters.waterHeight, 150 * 4, 150 * 4);

	_hudClockFace = new TGATexture(HUD_CLOCK_TEXTURE);
	_hudBullet = new TGATexture(HUD_BULLET_TEXTURE);
	_hudMissile = new TGATexture(HUD_MISSILE_TEXTURE);
	_hudRobot = new TGATexture(HUD_ROBOT_TEXTURE);
	_hudStatusBars = new TGATexture(HUD_STATUSBARS_TEXTURE);
	_hudLifebar = new TGATexture(HUD_LIFEBAR_TEXTURE);
	_hudShieldbar = new TGATexture(HUD_SHIELDBAR_TEXTURE);
	_hudEnergybar = new TGATexture(HUD_ENERGYBAR_TEXTURE);
	_hudReloadbar = new TGATexture(HUD_RELOADBAR_TEXTURE);
	_hudRadar = new TGATexture(HUD_RADAR_TEXTURE);

	//create human player's tank
	_playerTank = new SmallTank(*this, false);
	_playerTank->setPosition(_terrain->getRandomPointOnMap());
	_playerTank->setNeighbors();
	_targets.push_back(_playerTank);

	//add some AI tanks to the scene
	for (int i = 0; i < ENEMY_TANKS; i++) {
		Tank* tank = new SmallTank(*this, true);
		tank->setPosition(_terrain->getRandomPointOnMap());
		_targets.push_back(tank);
	}

	//add some  AI towers to the scene
	for (int i = 0; i < ENEMY_TOWERS; i++) {
		Tower* tower = new SmallTower(*this, true);
		tower->setPosition(_terrain->getRandomPointOnMap());
		_targets.push_back(tower);
	}

	// reset the scene
	reset();
}

void Scene::reset() {
	_renderingParameters.drawMode = RenderingParameters::POLYGON;
	_renderingParameters.shadeMode = RenderingParameters::SMOOTH;

	//the next update will be the first
	_firstUpdate = true;

	_cameraMode = TANK_CAM;
	_freeCameraParameters.reset();
	_freeCameraParameters.applyToCamera(*_tankCam);

	_skyDome->reset();
	_water->reset();

	for (std::vector<Projectile*>::iterator projectileIter = _projectiles.begin(); projectileIter != _projectiles.end();) {
		Projectile *projectile = *projectileIter;
		if (projectile->_projectileType == Projectile::BULLET) {
			Bullet* bullet = static_cast<Bullet*>(projectile);
			projectileIter = _projectiles.erase(projectileIter);
			delete bullet;
		} else if (projectile->_projectileType == Projectile::MISSILE) {
			Missile* missile = static_cast<Missile*>(projectile);

			projectileIter = _projectiles.erase(projectileIter);
			delete missile;
		} else if (projectile->_projectileType == Projectile::ROBOT) {
			Robot* robot = static_cast<Robot*>(projectile);

			projectileIter = _projectiles.erase(projectileIter);
			delete robot;
		} else {
			++projectileIter;
		}
	}

	for (std::vector<Target*>::iterator targetIter = _targets.begin(); targetIter != _targets.end(); ++targetIter) {
		Target* target = *targetIter;
		if (target->_targetType == Target::TANK) {
			Tank* tank = static_cast<Tank*>(target);
			tank->reset();
		} else if (target->_targetType == Target::TOWER) {
			Tower * tower = static_cast<Tower*>(target);
			tower->reset();
		}
	}

}

void Scene::update(float seconds) {

	//compile shader only when used and only once
	if (_shaderActive && !_shadersAlreadyCompiled) {
		_shadingEngine = ShadingEngine();
		_shadingEngine.compileShaders();
		_shadersAlreadyCompiled = true;
	}

	//soundengine listener position update
	_soundEngine.setListenerValues(_playerTank->getPosition().x, _playerTank->getPosition().y, _playerTank->getPosition().z);

	handleKeyboardInput();

	_skyDome->update(seconds);
	_water->update(seconds);

	//update target chooser if necessary
	if (_chooseTarget) {
		_targetChooser.y = _terrain->getHeight(_targetChooser);
	}

	for (std::vector<Target*>::iterator targetIter = _targets.begin(); targetIter != _targets.end();) {
		Target* target = *targetIter;
		if (target->_targetType == Target::TANK) {
			Tank* tank = static_cast<Tank*>(target);
			if (tank->isAIControlled()) {
				tank->getAI()->brainTick(seconds);
			}
			if (tank->_life < 0) {
				tank->reset();
			} else {
				++targetIter;
			}
			tank->update(seconds);
		} else if (target->_targetType == Target::TOWER) {
			Tower * tower = static_cast<Tower*>(target);
			if (tower->isAIControlled()) {
				tower->getAI()->brainTick(seconds);
			}
			if (tower->_life < 0) {
				tower->reset();
			} else {
				++targetIter;
			}
			tower->update(seconds);
		}
	}

	for (std::vector<Projectile*>::iterator projectileIter = _projectiles.begin(); projectileIter != _projectiles.end(); ++projectileIter) {
		Projectile *projectile = *projectileIter;
		if (projectile->_projectileType == Projectile::BULLET) {
			Bullet* bullet = static_cast<Bullet*>(projectile);
			bullet->move(seconds);
		} else if (projectile->_projectileType == Projectile::MISSILE) {
			Missile* missile = static_cast<Missile*>(projectile);
			missile->move(seconds);

		} else if (projectile->_projectileType == Projectile::ROBOT) {
			Robot* robot = static_cast<Robot*>(projectile);
			robot->move(seconds);
		}
	}

//detect collisions
	std::vector<Target*> targets = _targets;
	for (std::vector<Target*>::iterator targetIter = targets.begin(); targetIter != targets.end(); ++targetIter) {

		Target* currentTarget = (*targetIter);

		std::vector<Projectile*> projectiles = _projectiles;

		for (std::vector<Projectile*>::iterator projectileIter = projectiles.begin(); projectileIter != projectiles.end(); ++projectileIter) {

			Projectile* currentProjectile = (*projectileIter);

			if (currentTarget->_targetType == Target::TOWER) {
				continue;
			}

			if (currentTarget->checkHit(currentProjectile)) {
				_messageBus->sendMessageTo(AttackedByMessage(currentProjectile->_projectileOwner), currentTarget);
				switch (currentProjectile->_projectileType) {
				case Projectile::BULLET: {

					currentTarget->doDamage(BULLET_DAMAGE);
					break;
				}
				case Projectile::MISSILE: {
					currentTarget->doDamage(MISSILE_DAMAGE);
					break;
				}
				case Projectile::ROBOT: {
					currentTarget->doDamage(ROBOT_DAMAGE);
					break;
				}
				}
				currentProjectile->_detonated = true;
			}

		}

	}

	for (std::vector<Projectile*>::iterator projectileIter = _projectiles.begin(); projectileIter != _projectiles.end();) {
		Projectile *projectile = *projectileIter;
		if (projectile->_projectileType == Projectile::BULLET) {
			Bullet* bullet = static_cast<Bullet*>(projectile);

			if (bullet->isDetonated()) {
				projectileIter = _projectiles.erase(projectileIter);
				for (std::vector<Target*>::iterator targetIter = _targets.begin(); targetIter != _targets.end(); targetIter++) {
					_messageBus->sendMessageTo(DetonationSoundMessage(bullet->getPosition(), BULLET_DETONATIONSTRENGTH), *targetIter);
				}
				delete bullet;
			} else {
				++projectileIter;
			}
		} else if (projectile->_projectileType == Projectile::MISSILE) {
			Missile* missile = static_cast<Missile*>(projectile);
			if (missile->isDetonated()) {
				projectileIter = _projectiles.erase(projectileIter);
				for (std::vector<Target*>::iterator targetIter = _targets.begin(); targetIter != _targets.end(); targetIter++) {
					_messageBus->sendMessageTo(DetonationSoundMessage(missile->getPosition(), BULLET_DETONATIONSTRENGTH), *targetIter);
				}
				delete missile;
			} else {
				++projectileIter;
			}
		} else if (projectile->_projectileType == Projectile::ROBOT) {
			Robot* robot = static_cast<Robot*>(projectile);

			if (robot->isDetonated()) {
				projectileIter = _projectiles.erase(projectileIter);
				for (std::vector<Target*>::iterator targetIter = _targets.begin(); targetIter != _targets.end(); targetIter++) {
					_messageBus->sendMessageTo(DetonationSoundMessage(robot->getPosition(), BULLET_DETONATIONSTRENGTH), *targetIter);
				}
				delete robot;
			} else {
				++projectileIter;
			}
		} else {
			++projectileIter;
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

	//capture water reflection image
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Draw mirror image
	drawWaterImage();
	_water->capture();
	// Clear the screen
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//capture a picture for the map
	// Draw mirror image
	drawMap();

	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	_hudMap.capture(0, 0, height, height);
	// Clear the screen
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	if (_mirrorActive) {
		drawMirrorScene();
		_mirror.capture(0, 0, width, height);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	// Draw screen
	drawScene();

	// Draw overlay
	drawOverlay();

	// Swap buffers
	glFlush();
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

		Vector3D _tankDirection = Utils::rotate(_playerTank->getAzimuth(), Vector3D(0.0, 0.0, 1.0), Vector3D(0.0, 1.0, 0.0));
		Vector3D velocity(-_playerTank->getShootingPower() * std::cos(Utils::toRadian(_playerTank->getElevation())) * std::sin(Utils::toRadian(-_playerTank->getAzimuth())), _playerTank->getShootingPower() * std::sin(Utils::toRadian(_playerTank->getElevation())),
				-_playerTank->getShootingPower() * std::cos(Utils::toRadian(_playerTank->getElevation())) * std::cos(Utils::toRadian(-_playerTank->getAzimuth())));

		Point lookTo;
		lookTo.x = _playerTank->getPosition().x + velocity.x * 50;
		lookTo.y = _playerTank->getPosition().y + velocity.y * 50;
		lookTo.z = _playerTank->getPosition().z + velocity.z * 50;
		_currentlyActiveCamera->setLookAt(LookAt(Point(_playerTank->getPosition().x + _tankDirection.x * 50, _playerTank->getPosition().y + 50, _playerTank->getPosition().z + _tankDirection.z * 50), lookTo, Vector3D(0, 1, 0)));
	}

	// OpenGL camera
	_currentlyActiveCamera->setViewport(Viewport(0, 0, width, height));
	_currentlyActiveCamera->applyViewport();
	_currentlyActiveCamera->applyProjection();
	_currentlyActiveCamera->applyModelview();

	// Set scene parameters
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

	for (LightVector::iterator lightIter = _lights.begin(); lightIter != _lights.end(); ++lightIter) {
		Light *light = *lightIter;
		light->apply();
	}

	// draw sun light
	_sunLight->setPosition(_skyDome->getSunPosition());
	_sunLight->apply();

	// Draw scene
	glMatrixMode(GL_MODELVIEW);

	//#############################
	// Sky dome and water
	//The Sky dome and water should not use toon shading
	if (_shaderActive) {
		_shadingEngine.clearShaders();
	}

	// Draw the sky
	glTranslatef(0, -80, 0);
	_skyDome->setRenderingParameters(_renderingParameters);
	_skyDome->draw();
	glTranslatef(0, 80, 0);

	//###############################
	// Terrain
	if (_shaderActive) {
		_shadingEngine.clearShaders();
	}
	// Draw the terrain
	_terrain->setRenderingParameters(_renderingParameters);
	_terrain->draw();

	//################################
	// The targets should use toon shading as well
	if (_shaderActive) {
		_shadingEngine.applyToonShader();
	}

	//Draw the targets
	for (std::vector<Target*>::iterator targetIter = _targets.begin(); targetIter != _targets.end(); targetIter++) {
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

	if (_chooseTarget) {
		glPushMatrix();
		glTranslatef(_targetChooser.x, _targetChooser.y, _targetChooser.z);
		glutSolidCube(2);
		glPopMatrix();
	}

	//##################################
	// Projectiles
	//the projectiles should not use toon shading
	if (_shaderActive) {
		_shadingEngine.clearShaders();
	}
	for (std::vector<Projectile*>::iterator projectileIter = _projectiles.begin(); projectileIter != _projectiles.end(); ++projectileIter) {
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
	for (std::vector<Projectile*>::iterator projectileIter = _projectiles.begin(); projectileIter != _projectiles.end(); ++projectileIter) {
		Projectile *projectile = *projectileIter;
		if (projectile->_projectileType == Projectile::ROBOT) {
			Robot* robot = static_cast<Robot*>(projectile);
			robot->setRenderingParameters(_renderingParameters);
			robot->draw();
		}
	}

	//##################################
	// Shadowing
	//if shadows are active then draw shadows
	if (_shadowsActive) {
		GLmatrix16f Minv;
		//Point lightpos = Point(100, 100, 100);
		Point lightpos = _skyDome->getSunPosition();
		glClearDepth(1.0f); // Depth Buffer Setup
		glClearStencil(0); // Stencil Buffer Setup
		glEnable(GL_DEPTH_TEST); // Enables Depth Testing
		glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
		glCullFace(GL_BACK); // Set Culling Face To Back Face
		glEnable(GL_CULL_FACE); // Enable Culling

		glGetFloatv(GL_MODELVIEW_MATRIX, Minv); // Retrieve ModelView Matrix (Stores In Minv)
		VMatMult(Minv, lightpos); // We Store Rotated Light Vector In 'lp' Array
		Point wlp = Point(0, 0, 0, 1);
		lightpos.x += wlp.x;
		lightpos.y += wlp.y;
		lightpos.z += wlp.z;
		lightpos.w += wlp.w;

		_terrain->drawShadows(lightpos);
		//_playerTank->drawShadow(lightpos);
	}

	glEnable(GL_LIGHTING); // Enable Lighting
	glDepthMask(GL_TRUE); // Enable Depth Mask

	//############################
	// Fog
	if (_cameraMode == TANK_CAM) {
		if (_fogActive) {
			_fog->apply();
		} else {
			_fog->remove();
		}
	} else if (_cameraMode == OVERVIEW_CAM) {
		_fog->remove();
	}

	//draw water
	_water->setRenderingParameters(_renderingParameters);
	_water->draw();
}

void Scene::drawMirrorScene() {

	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	Vector3D velocity(-_playerTank->getShootingPower() * std::cos(Utils::toRadian(_playerTank->getElevation())) * std::sin(Utils::toRadian(-_playerTank->getAzimuth())), _playerTank->getShootingPower() * std::sin(Utils::toRadian(_playerTank->getElevation())),
			-_playerTank->getShootingPower() * std::cos(Utils::toRadian(_playerTank->getElevation())) * std::cos(Utils::toRadian(-_playerTank->getAzimuth())));

	Point lookTo;
	lookTo.x = _playerTank->getPosition().x + -velocity.x * 50;
	lookTo.y = _playerTank->getPosition().y + -velocity.y * 50;
	lookTo.z = _playerTank->getPosition().z + -velocity.z * 50;

	LookAt lookAt = _playerTank->getLookAt();
	lookAt.from.x = _playerTank->getPosition().x;
	lookAt.from.y = _playerTank->getPosition().y + 10;
	lookAt.from.z = _playerTank->getPosition().z;

	lookAt.to.x = lookTo.x;
	lookAt.to.z = lookTo.z;
	lookAt.to.y = lookAt.from.y;

	_currentlyActiveCamera = _tankCam;
	_currentlyActiveCamera->setLookAt(lookAt);

	// OpenGL camera
	_currentlyActiveCamera->setViewport(Viewport(0, 0, width, height));
	_currentlyActiveCamera->applyViewport();
	_currentlyActiveCamera->applyProjection();
	_currentlyActiveCamera->applyModelview();

	// Set scene parameters
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

	for (LightVector::iterator lightIter = _lights.begin(); lightIter != _lights.end(); ++lightIter) {
		Light *light = *lightIter;
		light->apply();
	}

	// draw sun light
	_sunLight->setPosition(_skyDome->getSunPosition());
	_sunLight->apply();

	// Draw scene
	glMatrixMode(GL_MODELVIEW);

	//#############################
	// Sky dome and water
	//The Sky dome and water should not use toon shading
	if (_shaderActive) {
		_shadingEngine.clearShaders();
	}

	// Draw the sky
	glTranslatef(0, -80, 0);
	_skyDome->setRenderingParameters(_renderingParameters);
	_skyDome->draw();
	glTranslatef(0, 80, 0);

	//###############################
	// Terrain
	if (_shaderActive) {
		_shadingEngine.clearShaders();
	}
	// Draw the terrain
	_terrain->setRenderingParameters(_renderingParameters);
	_terrain->draw();

	//draw water
	_water->setRenderingParameters(_renderingParameters);
	_water->draw();

	//################################
	// The targets should use toon shading as well
	if (_shaderActive) {
		_shadingEngine.applyToonShader();
	}

	//Draw the targets
	for (std::vector<Target*>::iterator targetIter = _targets.begin(); targetIter != _targets.end(); targetIter++) {
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

	//##################################
	// Projectiles
	//the projectiles should not use toon shading
	if (_shaderActive) {
		_shadingEngine.clearShaders();
	}
	for (std::vector<Projectile*>::iterator projectileIter = _projectiles.begin(); projectileIter != _projectiles.end(); ++projectileIter) {
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
	for (std::vector<Projectile*>::iterator projectileIter = _projectiles.begin(); projectileIter != _projectiles.end(); ++projectileIter) {
		Projectile *projectile = *projectileIter;
		if (projectile->_projectileType == Projectile::ROBOT) {
			Robot* robot = static_cast<Robot*>(projectile);
			robot->setRenderingParameters(_renderingParameters);
			robot->draw();
		}
	}

	glEnable(GL_LIGHTING); // Enable Lighting
	glDepthMask(GL_TRUE); // Enable Depth Mask

	//############################
	// Fog
	if (_cameraMode == TANK_CAM) {
		if (_fogActive) {
			_fog->apply();
		} else {
			_fog->remove();
		}
	}
}

void Scene::drawOverlay() {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	int margin = height / 30;

	// Set camera parameters
	_overlayCam->setViewport(Viewport(0, 0, width, height));

	_overlayCam->applyViewport();
	_overlayCam->applyProjection();
	_overlayCam->applyModelview();

	// Set overlay parameters
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// Draw overlay
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//clock
	_hudClockFace->setActive(true);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	float clocksize = height / 6;

	glTexCoord2f(0, _hudClockFace->getHeight());
	glVertex2f(margin, height - clocksize - margin);

	glTexCoord2f(_hudClockFace->getWidth(), _hudClockFace->getHeight());
	glVertex2f(margin + clocksize, height - clocksize - margin);

	glTexCoord2f(_hudClockFace->getWidth(), 0);
	glVertex2f(margin + clocksize, height - margin);

	glTexCoord2f(0, 0);
	glVertex2f(margin, height - margin);
	glEnd();
	_hudClockFace->setActive(false);

	glTranslatef(margin + clocksize / 2, height - clocksize / 2 - margin, 0);
	glRotatef(-_skyDome->getDisplacement(), 0, 0, 1);
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(clocksize / 30, 0);
	glVertex2f(0, clocksize / 2.66);
	glVertex2f(-clocksize / 30, 0);
	glEnd();

	glPopMatrix();

	// status bars
	glPushMatrix();
	_hudStatusBars->setActive(true);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	float statusBarsBaseSize = height / 6;
	float ratio = (float) _hudStatusBars->getWidth() / (float) _hudStatusBars->getHeight();

	glTexCoord2f(0, _hudStatusBars->getHeight());
	glVertex2f(margin, margin);

	glTexCoord2f(_hudStatusBars->getWidth(), _hudStatusBars->getHeight());
	glVertex2f(margin + statusBarsBaseSize * ratio, margin);

	glTexCoord2f(_hudStatusBars->getWidth(), 0);
	glVertex2f(margin + statusBarsBaseSize * ratio, statusBarsBaseSize + margin);

	glTexCoord2f(0, 0);
	glVertex2f(margin, statusBarsBaseSize + margin);
	glEnd();
	_hudStatusBars->setActive(false);

	//lifebar
	glPushMatrix();
	_hudLifebar->setActive(true);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	float statusBarBaseSize = height / 33;
	ratio = (float) _hudLifebar->getWidth() / (float) _hudLifebar->getHeight();

	glTexCoord2f(0, _hudLifebar->getHeight());
	glVertex2f(margin * 2.3, margin * 5.2);

	glTexCoord2f(_hudLifebar->getWidth(), _hudLifebar->getHeight());
	glVertex2f(margin * 2.3 + (_playerTank->_life / SMALLTANK_LIFE * statusBarBaseSize * ratio), margin * 5.2);

	glTexCoord2f(_hudLifebar->getWidth(), 0);
	glVertex2f(margin * 2.3 + (_playerTank->_life / SMALLTANK_LIFE * statusBarBaseSize * ratio), statusBarBaseSize + margin * 5.2);

	glTexCoord2f(0, 0);
	glVertex2f(margin * 2.3, statusBarBaseSize + margin * 5.2);
	glEnd();
	_hudLifebar->setActive(false);
	glPopMatrix();

	//shieldbar
	glPushMatrix();
	_hudShieldbar->setActive(true);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	ratio = (float) _hudShieldbar->getWidth() / (float) _hudShieldbar->getHeight();

	glTexCoord2f(0, _hudShieldbar->getHeight());
	glVertex2f(margin * 2.3, margin * 4);

	glTexCoord2f(_hudShieldbar->getWidth(), _hudShieldbar->getHeight());
	glVertex2f(margin * 2.3 + (_playerTank->_shield / SMALLTANK_SHIELD * statusBarBaseSize * ratio), margin * 4);

	glTexCoord2f(_hudShieldbar->getWidth(), 0);
	glVertex2f(margin * 2.3 + (_playerTank->_shield / SMALLTANK_SHIELD * statusBarBaseSize * ratio), statusBarBaseSize + margin * 4);

	glTexCoord2f(0, 0);
	glVertex2f(margin * 2.3, statusBarBaseSize + margin * 4);
	glEnd();
	_hudShieldbar->setActive(false);
	glPopMatrix();

	//energybar
	glPushMatrix();
	_hudEnergybar->setActive(true);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	ratio = (float) _hudEnergybar->getWidth() / (float) _hudEnergybar->getHeight();

	glTexCoord2f(0, _hudEnergybar->getHeight());
	glVertex2f(margin * 2.3, margin * 2.4);

	glTexCoord2f(_hudEnergybar->getWidth(), _hudEnergybar->getHeight());
	glVertex2f(margin * 2.3 + (_playerTank->getShootingPower() / SMALLTANK_MAXPOWER * statusBarBaseSize * ratio), margin * 2.4);

	glTexCoord2f(_hudEnergybar->getWidth(), 0);
	glVertex2f(margin * 2.3 + (_playerTank->getShootingPower() / SMALLTANK_MAXPOWER * statusBarBaseSize * ratio), statusBarBaseSize + margin * 2.4);

	glTexCoord2f(0, 0);
	glVertex2f(margin * 2.3, statusBarBaseSize + margin * 2.4);
	glEnd();
	_hudEnergybar->setActive(false);
	glPopMatrix();

	//reloadbar
	glPushMatrix();
	_hudReloadbar->setActive(true);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	ratio = (float) _hudReloadbar->getWidth() / (float) _hudReloadbar->getHeight();

	glTexCoord2f(0, _hudReloadbar->getHeight());
	glVertex2f(margin * 2.3, margin * 1.2);

	glTexCoord2f(_hudReloadbar->getWidth(), _hudReloadbar->getHeight());
	glVertex2f(margin * 2.3 + ((SMALLTANK_RELOADING_TIME - _playerTank->_reloadingTime) / SMALLTANK_RELOADING_TIME * statusBarBaseSize * ratio), margin * 1.2);

	glTexCoord2f(_hudReloadbar->getWidth(), 0);
	glVertex2f(margin * 2.3 + ((SMALLTANK_RELOADING_TIME - _playerTank->_reloadingTime) / SMALLTANK_RELOADING_TIME * statusBarBaseSize * ratio), statusBarBaseSize + margin * 1.2);

	glTexCoord2f(0, 0);
	glVertex2f(margin * 2.3, statusBarBaseSize + margin * 1.2);
	glEnd();
	_hudReloadbar->setActive(false);
	glPopMatrix();

	//currently active weapon
	TGATexture* selectedWeapon;
	switch (_playerTank->getSelectedWeapon()) {
	case Tank::BULLET: {
		selectedWeapon = _hudBullet;
		break;
	}
	case Tank::MISSILE: {
		selectedWeapon = _hudMissile;
		break;
	}
	case Tank::ROBOT: {
		selectedWeapon = _hudRobot;
		break;
	}

	}
	glPushMatrix();
	selectedWeapon->setActive(true);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	float selectedWeaponBaseSize = height / 6;
	ratio = (float) selectedWeapon->getWidth() / (float) selectedWeapon->getHeight();

	glTexCoord2f(0, selectedWeapon->getHeight());
	glVertex2f(width - margin - selectedWeaponBaseSize * ratio, margin);

	glTexCoord2f(selectedWeapon->getWidth(), selectedWeapon->getHeight());
	glVertex2f(width - margin, margin);

	glTexCoord2f(selectedWeapon->getWidth(), 0);
	glVertex2f(width - margin, selectedWeaponBaseSize + margin);

	glTexCoord2f(0, 0);
	glVertex2f(width - margin - selectedWeaponBaseSize * ratio, selectedWeaponBaseSize + margin);
	glEnd();

	selectedWeapon->setActive(false);
	glPopMatrix();

	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_hudRadar->setActive(true);
	glBegin(GL_POLYGON);

	float texBaseSize = height / 5;

	float left = 0.65;
	float up = 0.4;

	ratio = (float) _hudRadar->getWidth() / (float) _hudRadar->getHeight();

	glTexCoord2f(0, _hudRadar->getHeight());
	glVertex2f(width - margin * left - texBaseSize * ratio, height - margin * up);

	glTexCoord2f(_hudRadar->getWidth(), _hudRadar->getHeight());
	glVertex2f(width - margin * left, height - margin * up);

	glTexCoord2f(_hudRadar->getWidth(), 0);
	glVertex2f(width - margin * left, height - texBaseSize - margin * up);

	glTexCoord2f(0, 0);
	glVertex2f(width - margin * left - texBaseSize * ratio, height - texBaseSize - margin * up);
	glEnd();

	_hudRadar->setActive(false);
	glPopMatrix();

	//map
	glPushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	_hudMap.setActive(true);
	glBegin(GL_POLYGON);

	float mapBaseSize = height / 6;

	ratio = (float) _hudMap.getWidth() / (float) _hudMap.getHeight();

	glTexCoord2f(0, _hudMap.getHeight());
	glVertex2f(width - 2 * margin - mapBaseSize * ratio, height - margin);

	glTexCoord2f(_hudMap.getWidth(), _hudMap.getHeight());
	glVertex2f(width - 2 * margin, height - margin);

	glTexCoord2f(_hudMap.getWidth(), 0);
	glVertex2f(width - 2 * margin, height - mapBaseSize - margin);

	glTexCoord2f(0, 0);
	glVertex2f(width - 2 * margin - mapBaseSize * ratio, height - mapBaseSize - margin);
	glEnd();

	_hudMap.setActive(false);
	glPopMatrix();

	//Mirror
	if (_mirrorActive) {
		glPushMatrix();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		_mirror.setActive(true);
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);

		ratio = (float) _mirror.getWidth() / (float) _mirror.getHeight();

		glTexCoord2f(0, _mirror.getHeight());
		glVertex2f(width / 2, height - margin);

		glTexCoord2f(_mirror.getWidth(), _mirror.getHeight());
		glVertex2f(width / 2 + mapBaseSize * ratio, height - margin);

		glTexCoord2f(_mirror.getWidth(), 0);
		glVertex2f(width / 2 + mapBaseSize * ratio, height - mapBaseSize - margin);

		glTexCoord2f(0, 0);
		glVertex2f(width / 2, height - mapBaseSize - margin);
		glEnd();

		_mirror.setActive(false);
		glPopMatrix();
	}

	std::stringstream bulletTextStr;
	bulletTextStr << "inf.";
	std::stringstream missileTextStr;
	missileTextStr << _playerTank->_amountOfMissiles;
	std::stringstream robotTextStr;
	robotTextStr << _playerTank->_amountOfRobots;
	TextBox bulletText(*this, bulletTextStr.str().c_str(), 17, 1);
	TextBox missileText(*this, missileTextStr.str().c_str(), 17, 1);
	TextBox robotText(*this, robotTextStr.str().c_str(), 17, 1);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//number of other weapons
	glPushMatrix();
	//bullet
	_hudBullet->setActive(true);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	float iconBasesize = selectedWeaponBaseSize / 3;
	ratio = (float) _hudBullet->getWidth() / (float) _hudBullet->getHeight();

	glTexCoord2f(0, _hudBullet->getHeight());
	glVertex2f(width - margin - selectedWeaponBaseSize * ratio - iconBasesize * ratio, selectedWeaponBaseSize - iconBasesize + margin);

	glTexCoord2f(_hudBullet->getWidth(), _hudBullet->getHeight());
	glVertex2f(width - margin - selectedWeaponBaseSize * ratio, selectedWeaponBaseSize - iconBasesize + margin);

	glTexCoord2f(_hudBullet->getWidth(), 0);
	glVertex2f(width - margin - selectedWeaponBaseSize * ratio, selectedWeaponBaseSize + margin);

	glTexCoord2f(0, 0);
	glVertex2f(width - margin - selectedWeaponBaseSize * ratio - iconBasesize * ratio, selectedWeaponBaseSize + margin);
	glEnd();
	_hudBullet->setActive(false);

	glTranslatef(width - margin - selectedWeaponBaseSize * ratio - iconBasesize * 1.7 * ratio, selectedWeaponBaseSize - iconBasesize / 2 + margin, 0);
	bulletText.draw();
	glPopMatrix();

	glPushMatrix();
	//missile
	_hudMissile->setActive(true);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	ratio = (float) _hudMissile->getWidth() / (float) _hudMissile->getHeight();

	glTexCoord2f(0, _hudMissile->getHeight());
	glVertex2f(width - margin - selectedWeaponBaseSize * ratio - iconBasesize * ratio, selectedWeaponBaseSize - iconBasesize * 2 + margin);

	glTexCoord2f(_hudMissile->getWidth(), _hudMissile->getHeight());
	glVertex2f(width - margin - selectedWeaponBaseSize * ratio, selectedWeaponBaseSize - iconBasesize * 2 + margin);

	glTexCoord2f(_hudMissile->getWidth(), 0);
	glVertex2f(width - margin - selectedWeaponBaseSize * ratio, selectedWeaponBaseSize - iconBasesize + margin);

	glTexCoord2f(0, 0);
	glVertex2f(width - margin - selectedWeaponBaseSize * ratio - iconBasesize * ratio, selectedWeaponBaseSize - iconBasesize + margin);
	glEnd();
	_hudMissile->setActive(false);
	glTranslatef(width - margin - selectedWeaponBaseSize * ratio - iconBasesize * 1.7 * ratio, selectedWeaponBaseSize - iconBasesize * 3 / 2 + margin, 0);
	missileText.draw();
	glPopMatrix();

	glPushMatrix();
	//robot
	_hudRobot->setActive(true);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);

	ratio = (float) _hudRobot->getWidth() / (float) _hudRobot->getHeight();

	glTexCoord2f(0, _hudRobot->getHeight());
	glVertex2f(width - margin - selectedWeaponBaseSize * ratio - iconBasesize * ratio, selectedWeaponBaseSize - iconBasesize * 3 + margin);

	glTexCoord2f(_hudRobot->getWidth(), _hudRobot->getHeight());
	glVertex2f(width - margin - selectedWeaponBaseSize * ratio, selectedWeaponBaseSize - iconBasesize * 3 + margin);

	glTexCoord2f(_hudRobot->getWidth(), 0);
	glVertex2f(width - margin - selectedWeaponBaseSize * ratio, selectedWeaponBaseSize - iconBasesize * 2 + margin);

	glTexCoord2f(0, 0);
	glVertex2f(width - margin - selectedWeaponBaseSize * ratio - iconBasesize * ratio, selectedWeaponBaseSize - iconBasesize * 2 + margin);
	glEnd();
	_hudRobot->setActive(false);
	glTranslatef(width - margin - selectedWeaponBaseSize * ratio - iconBasesize * 1.7 * ratio, selectedWeaponBaseSize - iconBasesize * 5 / 2 + margin, 0);
	robotText.draw();
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

	if (_window.keyPressed('q') || _window.keyPressed('Q')) {
		float power = _playerTank->getShootingPower() + 0.1;
		_playerTank->setShootingPower((power > 1.0) ? 1.0 : power);
	}

	if (_window.keyPressed('e') || _window.keyPressed('E')) {
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

	if (_window.keyHit('3')) {
		_shaderActive = !_shaderActive;
	}

	if (_window.keyHit('4')) {
		if (_renderingParameters.normalMode == RenderingParameters::OFF) {
			_renderingParameters.normalMode = RenderingParameters::VERTEX;
		} else if (_renderingParameters.normalMode == RenderingParameters::VERTEX) {
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
		_soundActive = !_soundActive;
		_soundEngine.setActive(_soundActive);
	}

	if (_window.keyHit('9')) {
		_mirrorActive = !_mirrorActive;
	}
}

void Scene::onMouseEntry(int state) {
	if (state == GLUT_ENTERED) {
	} else if (state == GLUT_LEFT) {
	}
}

void Scene::onMouseClick(int button, int state, int x, int y) {

	if (state == GLUT_DOWN) {
		if (!_chooseTarget) {
			switch (button) {
			case GLUT_LEFT_BUTTON: {
				switch (_playerTank->getSelectedWeapon()) {
				case Tank::BULLET: {
					_playerTank->fireBullet();
					break;
				}
				case Tank::MISSILE: {
					if (_playerTank->_amountOfMissiles != 0) {
						_cameraMode = OVERVIEW_CAM;
						_targetChooser = _playerTank->getPosition();
						_chooseTarget = true;
					}

					break;
				}
				case Tank::ROBOT: {
					_playerTank->fireRobot();
					break;
				}
				default: {
					_playerTank->fireBullet();
					break;
				}
				}
				break;
			}
			case GLUT_RIGHT_BUTTON: {
				switch (_playerTank->getSelectedWeapon()) {
				case Tank::BULLET: {
					_playerTank->setSelectedWeapon(Tank::MISSILE);
					break;
				}
				case Tank::MISSILE: {
					_playerTank->setSelectedWeapon(Tank::ROBOT);
					break;
				}
				case Tank::ROBOT: {
					_playerTank->setSelectedWeapon(Tank::BULLET);
				}
				default: {
					_playerTank->setSelectedWeapon(Tank::BULLET);
				}
				}
				break;
			}
			}
		} else {
			_playerTank->fireMissile(_targetChooser);
			_chooseTarget = false;
			_cameraMode = TANK_CAM;
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
	if (abs(xMove) > 0 || abs(yMove) > 0) {
		glutWarpPointer(width / 2, height / 2);
	}

	if (!_chooseTarget) {
		_playerTank->setElevation(_playerTank->getElevation() - yMove * MOUSE_SENSITIVITY);
		_playerTank->setAzimuth(_playerTank->getAzimuth() + xMove * MOUSE_SENSITIVITY);
	} else {
		Vector3D _tankDirection = Utils::rotate(-_playerTank->getAzimuth(), Vector3D(0.0, 0.0, 1.0), Vector3D(0.0, 1.0, 0.0));
		Vector3D _cross = Utils::cross(_tankDirection, Vector3D(0, 1, 0));
		_targetChooser.x = _targetChooser.x - _tankDirection.x * yMove * MOUSE_SENSITIVITY / 2 - _cross.x * xMove * MOUSE_SENSITIVITY / 2;
		_targetChooser.z = _targetChooser.z + _tankDirection.z * yMove * MOUSE_SENSITIVITY / 2 + _cross.z * xMove * MOUSE_SENSITIVITY / 2;
	}
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
	Point from(radius * std::cos(Utils::toRadian(elevation)) * std::sin(Utils::toRadian(azimuth)) * -1, radius * std::sin(Utils::toRadian(elevation)), radius * std::cos(Utils::toRadian(elevation)) * std::cos(Utils::toRadian(azimuth)) * -1);

	Vector3D up(0.0, 1.0, 0.0);
	Vector3D dir(-from.x, -from.y, -from.z);
	Vector3D normal = Utils::cross(up, dir);

	Vector3D normalMove = normal;
	normalMove *= moveX;
	Vector3D upMove = up;
	upMove *= moveY;

	Point newFrom(from.x + normalMove.x + upMove.x, from.y + normalMove.y + upMove.y, from.z + normalMove.z + upMove.z);
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

Camera3D** Scene::getCurrentlyActiveCamera() {
	return &_currentlyActiveCamera;
}

MessageBus* Scene::getMessageBus() {
	return _messageBus;
}

/**
 * Water environment mapping part
 */

void Scene::drawWaterImage() {

	// Set camera parameters
	_water->applyCamera();

	// Set scene parameters
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

	for (LightVector::iterator lightIter = _lights.begin(); lightIter != _lights.end(); ++lightIter) {
		Light *light = *lightIter;
		light->apply();
	}

	// Draw scene
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	_skyDome->draw();
	glPopMatrix();
}

/**
 * Water environment mapping part
 */

void Scene::drawMap() {

	// Set camera parameters
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 1.0 / 1.0, 0.1, 2000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	int mapZoom = 50;
	Vector3D muzzleDirection = Vector3D(0, 0, 1);
	Vector3D tankDirection = Utils::rotate(_playerTank->getAzimuth() + 180, muzzleDirection, Vector3D(0, 1, 0));
	gluLookAt(_playerTank->getPosition().x, _playerTank->getPosition().y + mapZoom, _playerTank->getPosition().z, _playerTank->getPosition().x, _playerTank->getPosition().y, _playerTank->getPosition().z, tankDirection.x, tankDirection.y, tankDirection.z); //muzzleDirection.x, muzzleDirection.y, muzzleDirection.z);

	// Set scene parameters
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	// Draw scene
	glMatrixMode(GL_MODELVIEW);

	for (std::vector<Target*>::iterator targetIter = _targets.begin(); targetIter != _targets.end(); targetIter++) {
		Target* target = *targetIter;
		if (target->_targetType == Target::TANK) {
			glPushMatrix();
			glColor3f(0, 0.4, 0);
			glTranslatef(target->getPosition().x, target->getPosition().y, target->getPosition().z);
			glutSolidSphere(0.5, 12, 12);
			glRotatef(90, 1, 0, 0);
			glutSolidTorus(2, 3.5, 12, 12);
			glRotatef(-90, 1, 0, 0);
			glPopMatrix();
		} else {

			glPushMatrix();
			glColor3f(0, 0.4, 0);
			glTranslatef(target->getPosition().x, target->getPosition().y, target->getPosition().z);
			glutSolidCube(5);
			glPopMatrix();
		}
	}
}

Water* Scene::getWater() {
	return _water;
}

std::vector<Target*> Scene::getTargets() {
	return _targets;
}
}
