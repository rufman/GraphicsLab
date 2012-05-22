/*
 * menu.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */


#ifndef MENU_HPP
#define MENU_HPP

//Includes
#include "defs.hpp"
#include "gl_includes.hpp"
#include "drawable.hpp"
#include "tgaTexture.hpp"
#include "button.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN


// Forward declaration
class Button;
typedef std::vector <Button*> ButtonList;

enum MenuType {
	MAIN_MENU,
	STANDARD_MENU,
	SAVE_MENU,
	LOAD_MENU,
	SETTINGS_MENU
};

class Menu : public Drawable {

public:
	Menu(Scene &scene_, MenuType type_);
	~Menu();
	
	void addButton(Button *button_);
	
	void update();
	
	void draw();
	
	int getOffsetX();
	int getOffsetY();

private:
	
	void updateOffsets();

	static const float MENU_OVERLAY_ALPHA = 0.5;

	static const float MENU_TEXTURE_X = 600.0;
	static const float MENU_TEXTURE_Y = 0.0;
	static const float MENU_TEXTURE_WIDTH = 420.0;
	static const float MENU_TEXTURE_HEIGHT = 620.0;

	static const float MENU_LOGO_X = 60.0;
	static const float MENU_LOGO_Y = 405.0;
	static const float MENU_LOGO_TEXTURE_X = 300.0;
	static const float MENU_LOGO_TEXTURE_Y = 850.0;
	static const float MENU_LOGO_TEXTURE_WIDTH = 300.0;
	static const float MENU_LOGO_TEXTURE_HEIGHT = 170.0;

	static const float MENU_TITLE_X = 60.0;
	static const float MENU_TITLE_Y = 530.0;
	static const float MENU_TITLE_TEXTURE_X = 0.0;
	static const float MENU_TITLE_SAVE_GAME_TEXTURE_Y = 850.0;
	static const float MENU_TITLE_LOAD_GAME_TEXTURE_Y = 900.0;
	static const float MENU_TITLE_SETTINGS_TEXTURE_Y = 950.0;
	static const float MENU_TITLE_TEXTURE_WIDTH = 300.0;
	static const float MENU_TITLE_TEXTURE_HEIGHT = 50.0;

	void drawButtons();

	MenuType _type;

	int _offsetX;
	int _offsetY;

	ButtonList _buttonList;
	Button* _highLightedButton;
	
};

GRAPHICSLAB_NAMESPACE_END
#endif  // GRAPHICSLAB_MENU_HPP
