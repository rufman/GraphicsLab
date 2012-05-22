/*
 * menuManager.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "menuManager.hpp"
#include "menuFunctions.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

bool MenuManager::_open;
MenuState MenuManager::_state;
TGATexture* MenuManager::_texture;
bool MenuManager::_introMenuMode;

Button* MenuManager::_viewDistanceButton;
Button* MenuManager::_geometryQualityButton;
Button* MenuManager::_dynamicReflectionButton;
Button* MenuManager::_dynamicShadowsButton;
Button* MenuManager::_herbageButton;
Button* MenuManager::_fullscreenButton;

MenuManager::MenuManager(Scene& scene) :
	_scene(scene),
	_mainMenu(NULL),
	_ingameMenu(NULL),
	_saveMenu(NULL),
	_loadMenu(NULL),
	_settingsMenu(NULL)
{
	_state = NONE;
	_open = true;
	_texture = NULL;
	_introMenuMode = false;

	_viewDistanceButton = NULL;
	_geometryQualityButton = NULL;
	_dynamicReflectionButton = NULL;
	_dynamicShadowsButton = NULL;
	_herbageButton = NULL;
	_fullscreenButton = NULL;
}

MenuManager::~MenuManager() {
	delete _mainMenu;
	delete _ingameMenu;
	delete _saveMenu;
	delete _loadMenu;
	delete _settingsMenu;

	delete _texture;

	delete _viewDistanceButton;
	delete _geometryQualityButton;
	delete _dynamicReflectionButton;
	delete _dynamicShadowsButton;
	delete _herbageButton;
	delete _fullscreenButton;
}

void MenuManager::initialize() {

	// Texture
	_texture = new TGATexture("data/textures/menu.tga");
	_texture->init();



	// Main menu
	_mainMenu = new Menu(_scene, MAIN_MENU);

	Button* newGameButton = new Button(_scene, WHITE);
	newGameButton->setPosition(50, 310);
	newGameButton->setTextTextureCoordinates(0, 250);
	newGameButton->setOnClickFunction(newGameButtonFunction);
	_mainMenu->addButton(newGameButton);

	Button* loadGameButton1 = new Button(_scene, WHITE);
	loadGameButton1->setPosition(50, 240);
	loadGameButton1->setTextTextureCoordinates(0, 50);
	loadGameButton1->setOnClickFunction(loadGameButtonFunction);
	_mainMenu->addButton(loadGameButton1);

	Button* settingsButton1 = new Button(_scene, WHITE);
	settingsButton1->setPosition(50, 170);
	settingsButton1->setTextTextureCoordinates(0, 100);
	settingsButton1->setOnClickFunction(settingsButtonFunction);
	_mainMenu->addButton(settingsButton1);

	Button* quitButton = new Button(_scene, RED);
	quitButton->setPosition(50, 50);
	quitButton->setTextTextureCoordinates(0, 300);
	quitButton->setOnClickFunction(quitButtonFunction);
	_mainMenu->addButton(quitButton);



	// Load game menu
	_loadMenu = new Menu(_scene, LOAD_MENU);

	Button* loadGameSave1Button = new Button(_scene, WHITE);
	loadGameSave1Button->setPosition(50, 450);
	loadGameSave1Button->setTextTextureCoordinates(0, 400);
	loadGameSave1Button->setOnClickFunction(loadGameSave1ButtonFunction);
	_loadMenu->addButton(loadGameSave1Button);

	Button* loadGameSave2Button = new Button(_scene, WHITE);
	loadGameSave2Button->setPosition(50, 380);
	loadGameSave2Button->setTextTextureCoordinates(0, 450);
	loadGameSave2Button->setOnClickFunction(loadGameSave2ButtonFunction);
	_loadMenu->addButton(loadGameSave2Button);

	Button* loadGameSave3Button = new Button(_scene, WHITE);
	loadGameSave3Button->setPosition(50, 310);
	loadGameSave3Button->setTextTextureCoordinates(0, 500);
	loadGameSave3Button->setOnClickFunction(loadGameSave3ButtonFunction);
	_loadMenu->addButton(loadGameSave3Button);

	Button* loadGameSave4Button = new Button(_scene, WHITE);
	loadGameSave4Button->setPosition(50, 240);
	loadGameSave4Button->setTextTextureCoordinates(0, 550);
	loadGameSave4Button->setOnClickFunction(loadGameSave4ButtonFunction);
	_loadMenu->addButton(loadGameSave4Button);

	Button* loadGameSave5Button = new Button(_scene, WHITE);
	loadGameSave5Button->setPosition(50, 170);
	loadGameSave5Button->setTextTextureCoordinates(0, 600);
	loadGameSave5Button->setOnClickFunction(loadGameSave5ButtonFunction);
	_loadMenu->addButton(loadGameSave5Button);

	Button* cancelButton1 = new Button(_scene, RED);
	cancelButton1->setPosition(50, 50);
	cancelButton1->setTextTextureCoordinates(0, 350);
	cancelButton1->setOnClickFunction(returnToMenuButtonFunction);
	_loadMenu->addButton(cancelButton1);



	// Save game menu
	_saveMenu = new Menu(_scene, SAVE_MENU);

	Button* saveGameSave1Button = new Button(_scene, WHITE);
	saveGameSave1Button->setPosition(50, 450);
	saveGameSave1Button->setTextTextureCoordinates(0, 400);
	saveGameSave1Button->setOnClickFunction(saveGameSave1ButtonFunction);
	_saveMenu->addButton(saveGameSave1Button);

	Button* saveGameSave2Button = new Button(_scene, WHITE);
	saveGameSave2Button->setPosition(50, 380);
	saveGameSave2Button->setTextTextureCoordinates(0, 450);
	saveGameSave2Button->setOnClickFunction(saveGameSave2ButtonFunction);
	_saveMenu->addButton(saveGameSave2Button);

	Button* saveGameSave3Button = new Button(_scene, WHITE);
	saveGameSave3Button->setPosition(50, 310);
	saveGameSave3Button->setTextTextureCoordinates(0, 500);
	saveGameSave3Button->setOnClickFunction(saveGameSave3ButtonFunction);
	_saveMenu->addButton(saveGameSave3Button);

	Button* saveGameSave4Button = new Button(_scene, WHITE);
	saveGameSave4Button->setPosition(50, 240);
	saveGameSave4Button->setTextTextureCoordinates(0, 550);
	saveGameSave4Button->setOnClickFunction(saveGameSave4ButtonFunction);
	_saveMenu->addButton(saveGameSave4Button);

	Button* saveGameSave5Button = new Button(_scene, WHITE);
	saveGameSave5Button->setPosition(50, 170);
	saveGameSave5Button->setTextTextureCoordinates(0, 600);
	saveGameSave5Button->setOnClickFunction(saveGameSave5ButtonFunction);
	_saveMenu->addButton(saveGameSave5Button);

	Button* cancelButton2 = new Button(_scene, RED);
	cancelButton2->setPosition(50, 50);
	cancelButton2->setTextTextureCoordinates(0, 350);
	cancelButton2->setOnClickFunction(returnToMenuButtonFunction);
	_saveMenu->addButton(cancelButton2);



	// Settings menu
	_settingsMenu = new Menu(_scene, SETTINGS_MENU);

	_viewDistanceButton = new Button(_scene, WHITE);
	_viewDistanceButton->setPosition(50, 450);
	_viewDistanceButton->setTextTextureCoordinates(0, 400);
	_viewDistanceButton->setOnClickFunction(viewDistanceButtonFunction);
	_settingsMenu->addButton(_viewDistanceButton);

	_geometryQualityButton = new Button(_scene, WHITE);
	_geometryQualityButton->setPosition(50, 390);
	_geometryQualityButton->setTextTextureCoordinates(0, 400);
	_geometryQualityButton->setOnClickFunction(geometryQualityButtonFunction);
	_settingsMenu->addButton(_geometryQualityButton);

	_dynamicReflectionButton = new Button(_scene, WHITE);
	_dynamicReflectionButton->setPosition(50, 330);
	_dynamicReflectionButton->setTextTextureCoordinates(0, 400);
	_dynamicReflectionButton->setOnClickFunction(dynamicReflectionButtonFunction);
	_settingsMenu->addButton(_dynamicReflectionButton);

	_dynamicShadowsButton = new Button(_scene, WHITE);
	_dynamicShadowsButton->setPosition(50, 270);
	_dynamicShadowsButton->setTextTextureCoordinates(0, 400);
	_dynamicShadowsButton->setOnClickFunction(dynamicShadowsButtonFunction);
	_settingsMenu->addButton(_dynamicShadowsButton);

	_herbageButton = new Button(_scene, WHITE);
	_herbageButton->setPosition(50, 210);
	_herbageButton->setTextTextureCoordinates(0, 400);
	_herbageButton->setOnClickFunction(herbageButtonFunction);
	_settingsMenu->addButton(_herbageButton);

	_fullscreenButton = new Button(_scene, WHITE);
	_fullscreenButton->setPosition(50, 150);
	_fullscreenButton->setTextTextureCoordinates(0, 400);
	_fullscreenButton->setOnClickFunction(fullscreenButtonFunction);
	_settingsMenu->addButton(_fullscreenButton);

	Button* saveButton = new Button(_scene, RED);
	saveButton->setPosition(50, 50);
	saveButton->setTextTextureCoordinates(0, 650);
	saveButton->setOnClickFunction(saveSettingsButtonFunction);
	_settingsMenu->addButton(saveButton);



	// Ingame menu
	_ingameMenu = new Menu(_scene, STANDARD_MENU);

	Button* saveGameButton = new Button(_scene, WHITE);
	saveGameButton->setPosition(50, 500);
	saveGameButton->setTextTextureCoordinates(0, 0);
	saveGameButton->setOnClickFunction(saveGameButtonFunction);
	_ingameMenu->addButton(saveGameButton);

	Button* loadGameButton2 = new Button(_scene, WHITE);
	loadGameButton2->setPosition(50, 430);
	loadGameButton2->setTextTextureCoordinates(0, 50);
	loadGameButton2->setOnClickFunction(loadGameButtonFunction);
	_ingameMenu->addButton(loadGameButton2);

	Button* settingsButton2 = new Button(_scene, WHITE);
	settingsButton2->setPosition(50, 360);
	settingsButton2->setTextTextureCoordinates(0, 100);
	settingsButton2->setOnClickFunction(settingsButtonFunction);
	_ingameMenu->addButton(settingsButton2);

	Button* quitGameButton = new Button(_scene, WHITE);
	quitGameButton->setPosition(50, 290);
	quitGameButton->setTextTextureCoordinates(0, 200);
	quitGameButton->setOnClickFunction(quitGameButtonFunction);
	_ingameMenu->addButton(quitGameButton);

	Button* returnToGameButton = new Button(_scene, RED);
	returnToGameButton->setPosition(50, 50);
	returnToGameButton->setTextTextureCoordinates(0, 150);
	returnToGameButton->setOnClickFunction(returnToGameButtonFunction);
	_ingameMenu->addButton(returnToGameButton);


	_state = MAIN;
	_introMenuMode = true;
}

void MenuManager::draw() {
	if(_state != NONE) {
		glPushMatrix();
		switch(_state) {
			case MAIN:
				_mainMenu->draw();
				break;
			case INGAME:
				_ingameMenu->draw();
				break;
			case SAVE:
				_saveMenu->draw();
				break;
			case LOAD:
				_loadMenu->draw();
				break;
			case SETTINGS:
				_settingsMenu->draw();
				break;
		}
		glPopMatrix();
	}
}

void MenuManager::update() {
	handleKeyInput();
	if(_state != NONE) {
		switch(_state) {
			case MAIN:
				_mainMenu->update();
				break;
			case INGAME:
				_ingameMenu->update();
				break;
			case SAVE:
				_saveMenu->update();
				break;
			case LOAD:
				_loadMenu->update();
				break;
			case SETTINGS:
				_settingsMenu->update();
				break;
		}
	}
}

bool MenuManager::isMenuOpen() {
	return _open;
}

void MenuManager::changeMenuState(MenuState state) {
	if(state == NONE) {
		_open = false;
	} else {
		_open = true;
	}
	_state = state;
}

void MenuManager::handleKeyInput() {
	switch(_state) {
		case NONE:
			if(InputManager::keyDown(27)) {
				changeMenuState(INGAME);
			}
			break;
		case INGAME:
			if(InputManager::keyDown(27)) {
				changeMenuState(NONE);
			}
			break;
	}
}

TGATexture* MenuManager::getMenuTexture() {
	return _texture;
}

bool MenuManager::isInIntroMenuMode() {
	return _introMenuMode;
}

void MenuManager::setIntroMenuMode(bool mode) {
	_introMenuMode = mode;
}

void MenuManager::updateViewDistanceButton() {
	switch(GraphicsSettings::getSectorRenderLimit()) {
		case 8:
			_viewDistanceButton->setTextTextureCoordinates(300, 0);
			break;
		case 12:
			_viewDistanceButton->setTextTextureCoordinates(300, 50);
			break;
		case 16:
			_viewDistanceButton->setTextTextureCoordinates(300, 100);
			break;
	}
}

void MenuManager::updateGeometryQualityButton() {
	switch(GraphicsSettings::getHighestQuality()) {
		case 1:
			_geometryQualityButton->setTextTextureCoordinates(300, 150);
			break;
		case 2:
			_geometryQualityButton->setTextTextureCoordinates(300, 200);
			break;
		case 3:
			_geometryQualityButton->setTextTextureCoordinates(300, 250);
			break;
	}
}

void MenuManager::updateDynamicReflectionButton() {
	switch(GraphicsSettings::getReflections()) {
		case 1:
			_dynamicReflectionButton->setTextTextureCoordinates(300, 300);
			break;
		case 0:
			_dynamicReflectionButton->setTextTextureCoordinates(300, 350);
			break;
	}
}

void MenuManager::updateDynamicShadowsButton() {
	switch(GraphicsSettings::getShadows()) {
		case 1:
			_dynamicShadowsButton->setTextTextureCoordinates(300, 400);
			break;
		case 0:
			_dynamicShadowsButton->setTextTextureCoordinates(300, 450);
			break;
	}
}

void MenuManager::updateHerbageButton() {
	switch(GraphicsSettings::getGrass()) {
		case 1:
			_herbageButton->setTextTextureCoordinates(300, 600);
			break;
		case 0:
			_herbageButton->setTextTextureCoordinates(300, 650);
			break;
	}
}

void MenuManager::updateFullscreenButton() {
	switch(GraphicsSettings::getTempFullscreen()) {
		case 1:
			_fullscreenButton->setTextTextureCoordinates(300, 500);
			break;
		case 0:
			_fullscreenButton->setTextTextureCoordinates(300, 550);
			break;
	}
}


GRAPHICSLAB_NAMESPACE_END
