/*
 * menuManager.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef MENUMANAGER_HPP_
#define MENUMANAGER_HPP_


//Includes
#include "defs.hpp"
#include "scene.hpp"
#include "menu.hpp"
#include "button.hpp"
#include "tgaTexture.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

// Forward declarations
class Menu;
class Button;

enum MenuState {
	NONE,
	MAIN,
	INGAME,
	SAVE,
	LOAD,
	SETTINGS
};


class MenuManager {

public:

	MenuManager(Scene& scene);

	~MenuManager();

	void initialize();

	void draw();

	void update();

	static bool isMenuOpen();

	static void changeMenuState(MenuState state);
	static MenuState getMenuState(){return _state;};
	void handleKeyInput();

	static bool isInIntroMenuMode();

	static TGATexture* getMenuTexture();
	static void setIntroMenuMode(bool mode);

	static void updateViewDistanceButton();
	static void updateGeometryQualityButton();
	static void updateDynamicReflectionButton();
	static void updateDynamicShadowsButton();
	static void updateHerbageButton();
	static void updateFullscreenButton();

private:

	Scene& _scene;

	static MenuState _state;

	static bool _open;

	static bool _introMenuMode;

	//Menus
	Menu* _mainMenu;
	Menu* _ingameMenu;
	Menu* _saveMenu;
	Menu* _loadMenu;
	Menu* _settingsMenu;

	//Settings Buttons
	static Button* _viewDistanceButton;
	static Button* _geometryQualityButton;
	static Button* _dynamicReflectionButton;
	static Button* _dynamicShadowsButton;
	static Button* _herbageButton;
	static Button* _fullscreenButton;

	static TGATexture* _texture;
};


GRAPHICSLAB_NAMESPACE_END

#endif /* MENUMANAGER_HPP_ */
