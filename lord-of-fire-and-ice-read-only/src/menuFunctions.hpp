/**
 * menuFunctions.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef MENUFUNCTIONS_HPP_
#define MENUFUNCTIONS_HPP_

#include "scene.hpp"
#include "menuManager.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN
class Scene;
class Menumanager;

void newGameButtonFunction(Scene& s_) {
	s_.loadGame(0);
}

void saveGameButtonFunction(Scene& s_) {
	MenuManager::changeMenuState(SAVE);
}

void loadGameButtonFunction(Scene& s_) {
	MenuManager::changeMenuState(LOAD);
}

void settingsButtonFunction(Scene& s_) {
	MenuManager::changeMenuState(SETTINGS);
}

void quitGameButtonFunction(Scene& s_) {
	s_.loadIntroMap();
}

void quitButtonFunction(Scene& s_) {
	exit(0);
}

void returnToGameButtonFunction(Scene& s_) {
	MenuManager::changeMenuState(NONE);
}

void returnToMenuButtonFunction(Scene& s_) {
	if(MenuManager::isInIntroMenuMode()) {
		MenuManager::changeMenuState(MAIN);
	} else {
		MenuManager::changeMenuState(INGAME);
	}
}

void saveSettingsButtonFunction(Scene& s_) {
	GraphicsSettings::save();
	if(MenuManager::isInIntroMenuMode()) {
		MenuManager::changeMenuState(MAIN);
	} else {
		MenuManager::changeMenuState(INGAME);
	}
}

void loadGameSave1ButtonFunction(Scene& s_) {
	s_.loadGame(1);
}

void loadGameSave2ButtonFunction(Scene& s_) {
	s_.loadGame(2);
}

void loadGameSave3ButtonFunction(Scene& s_) {
	s_.loadGame(3);
}

void loadGameSave4ButtonFunction(Scene& s_) {
	s_.loadGame(4);
}

void loadGameSave5ButtonFunction(Scene& s_) {
	s_.loadGame(5);
}

void saveGameSave1ButtonFunction(Scene& s_) {
	s_.saveGame(1);
}

void saveGameSave2ButtonFunction(Scene& s_) {
	s_.saveGame(2);
}

void saveGameSave3ButtonFunction(Scene& s_) {
	s_.saveGame(3);
}

void saveGameSave4ButtonFunction(Scene& s_) {
	s_.saveGame(4);
}

void saveGameSave5ButtonFunction(Scene& s_) {
	s_.saveGame(5);
}

void viewDistanceButtonFunction(Scene& s_) {
	GraphicsSettings::changeSectorRenderLimit();
}

void geometryQualityButtonFunction(Scene& s_) {
	GraphicsSettings::changeHighestQuality();
}

void dynamicReflectionButtonFunction(Scene& s_) {
	GraphicsSettings::changeReflections();
}

void dynamicShadowsButtonFunction(Scene& s_) {
	GraphicsSettings::changeShadows();
}

void herbageButtonFunction(Scene& s_) {
	GraphicsSettings::changeGrass();
}

void fullscreenButtonFunction(Scene& s_) {
	GraphicsSettings::changeFullscreen();
}


GRAPHICSLAB_NAMESPACE_END

#endif /* MENUFUNCTIONS_HPP_ */
