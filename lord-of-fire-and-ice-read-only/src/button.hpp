/*
 * button.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef BUTTON_HPP
#define BUTTON_HPP

//Includes
#include "defs.hpp"
#include "gl_includes.hpp"
#include "drawable.hpp"
#include "tgaTexture.hpp"
#include "vector.hpp"
#include "textBox.hpp"
#include "menuManager.hpp"
#include "menu.hpp"


typedef int (*Function)();

GRAPHICSLAB_NAMESPACE_BEGIN

enum ButtonType {
	WHITE,
	RED
};


// Forward declarations
class MenuManager;

class Button : public Drawable
{
public:
	Button();
	/** Draw the object to the screen. */
	Button(Scene& scene_, ButtonType type_);
	~Button();
	void draw() const;
	
	void setPosition(int x_, int y_);
	void setTextTextureCoordinates(int x_, int y_);
	void setOnClickFunction(void (*f)(Scene&));
	
	int getX(){return _x;	};
	int getY(){return _y;	};
	
	//Mouse interaction
	bool isMouseInside(Menu* menu);
	void onClick();
	void offClick();
	void rollOver();
	void rollOut();
	
	
private:
	static const float BUTTON_WIDTH = 320.0;
	static const float BUTTON_HEIGHT = 70.0;

	static const float BUTTON_TO_TEXT_OFFSET_X = 10.0;
	static const float BUTTON_TO_TEXT_OFFSET_Y = 10.0;

	static const float BUTTON_TEXT_TEXTURE_WIDTH = 300.0;
	static const float BUTTON_TEXT_TEXTURE_HEIGHT = 50.0;

	static const float BUTTON_OUT_TEXTURE_X = 650.0;
	static const float BUTTON_OUT_TEXTURE_Y = 620.0;

	static const float BUTTON_OVER_TEXTURE_X = 650.0;
	static const float BUTTON_OVER_TEXTURE_Y = 690.0;

	static const float BUTTON_RED_OUT_TEXTURE_X = 650.0;
	static const float BUTTON_RED_OUT_TEXTURE_Y = 760.0;

	static const float BUTTON_RED_OVER_TEXTURE_X = 650.0;
	static const float BUTTON_RED_OVER_TEXTURE_Y = 830.0;

	static const float BUTTON_SHADOW_WIDTH = 10.0;

	int _x, _y;

	int _textTextureX, _textTextureY;

	ButtonType _type;

	bool _mouseHover;

	void (*_onClickFunction)(Scene&);
};

GRAPHICSLAB_NAMESPACE_END
#endif  // GRAPHICSLAB_BUTTON_HPP
