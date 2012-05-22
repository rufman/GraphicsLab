/*
 * button.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "button.hpp"

// Includes
#include "gl_includes.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN

Button::Button(Scene& scene_, ButtonType type_) :
	Drawable(scene_, 0,0,0),
	_x(0),
	_y(0),
	_textTextureX(0),
	_textTextureY(0),
	_type(type_),
	_mouseHover(false) {
}


Button::~Button() {
	
}

void Button::setPosition(int x_, int y_) {
	_x = x_;
	_y = y_;
}

void Button::setTextTextureCoordinates(int x_, int y_) {
	_textTextureX = x_;
	_textTextureY = y_;
}

void Button::setOnClickFunction(void (*f)(Scene&)) {
	_onClickFunction = f;
}

bool Button::isMouseInside(Menu* menu) {
	int mouseX = InputManager::mouseX();
	int mouseY = glutGet(GLUT_WINDOW_HEIGHT) - InputManager::mouseY();
	int offsetX = menu->getOffsetX() + _x;
	int offsetY = menu->getOffsetY() + _y;
	if(mouseX > offsetX + BUTTON_SHADOW_WIDTH
		 && mouseX <  offsetX + BUTTON_WIDTH - BUTTON_SHADOW_WIDTH
		 && mouseY > offsetY + BUTTON_SHADOW_WIDTH
		 && mouseY < offsetY + BUTTON_HEIGHT - BUTTON_SHADOW_WIDTH) {
		return true;
	}
	return false;
}

void Button::onClick() {
	_onClickFunction(_scene);
}

void Button::rollOver() {
	_mouseHover = true;
}

void Button::rollOut() {
	_mouseHover = false;
}

void Button::draw() const {

 	glColor4f(1.0, 1.0, 1.0, 1.0);

 	MenuManager::getMenuTexture()->setActive(true);
	glBegin(GL_QUADS);

	if(_type == WHITE) {
		if(_mouseHover) {
			glTexCoord2f(BUTTON_OVER_TEXTURE_X,                BUTTON_OVER_TEXTURE_Y);
			glVertex2f(_x,                _y + BUTTON_HEIGHT);

			glTexCoord2f(BUTTON_OVER_TEXTURE_X + BUTTON_WIDTH, BUTTON_OVER_TEXTURE_Y);
			glVertex2f(_x + BUTTON_WIDTH, _y + BUTTON_HEIGHT);

			glTexCoord2f(BUTTON_OVER_TEXTURE_X + BUTTON_WIDTH, BUTTON_OVER_TEXTURE_Y + BUTTON_HEIGHT);
			glVertex2f(_x + BUTTON_WIDTH, _y);

			glTexCoord2f(BUTTON_OVER_TEXTURE_X,                BUTTON_OVER_TEXTURE_Y + BUTTON_HEIGHT);
			glVertex2f(_x,                _y );

		} else {
			glTexCoord2f(BUTTON_OUT_TEXTURE_X,                BUTTON_OUT_TEXTURE_Y);
			glVertex2f(_x,                _y + BUTTON_HEIGHT);

			glTexCoord2f(BUTTON_OUT_TEXTURE_X + BUTTON_WIDTH, BUTTON_OUT_TEXTURE_Y);
			glVertex2f(_x + BUTTON_WIDTH, _y + BUTTON_HEIGHT);

			glTexCoord2f(BUTTON_OUT_TEXTURE_X + BUTTON_WIDTH, BUTTON_OUT_TEXTURE_Y + BUTTON_HEIGHT);
			glVertex2f(_x + BUTTON_WIDTH, _y);

			glTexCoord2f(BUTTON_OUT_TEXTURE_X,                BUTTON_OUT_TEXTURE_Y + BUTTON_HEIGHT);
			glVertex2f(_x,                _y );
		}


	} else if(_type == RED) {
		if(_mouseHover) {
			glTexCoord2f(BUTTON_RED_OVER_TEXTURE_X,                BUTTON_RED_OVER_TEXTURE_Y);
			glVertex2f(_x,                _y + BUTTON_HEIGHT);

			glTexCoord2f(BUTTON_RED_OVER_TEXTURE_X + BUTTON_WIDTH, BUTTON_RED_OVER_TEXTURE_Y);
			glVertex2f(_x + BUTTON_WIDTH, _y + BUTTON_HEIGHT);

			glTexCoord2f(BUTTON_RED_OVER_TEXTURE_X + BUTTON_WIDTH, BUTTON_RED_OVER_TEXTURE_Y + BUTTON_HEIGHT);
			glVertex2f(_x + BUTTON_WIDTH, _y);

			glTexCoord2f(BUTTON_RED_OVER_TEXTURE_X,                BUTTON_RED_OVER_TEXTURE_Y + BUTTON_HEIGHT);
			glVertex2f(_x,                _y );

		} else {
			glTexCoord2f(BUTTON_RED_OUT_TEXTURE_X,                BUTTON_RED_OUT_TEXTURE_Y);
			glVertex2f(_x,                _y + BUTTON_HEIGHT);

			glTexCoord2f(BUTTON_RED_OUT_TEXTURE_X + BUTTON_WIDTH, BUTTON_RED_OUT_TEXTURE_Y);
			glVertex2f(_x + BUTTON_WIDTH, _y + BUTTON_HEIGHT);

			glTexCoord2f(BUTTON_RED_OUT_TEXTURE_X + BUTTON_WIDTH, BUTTON_RED_OUT_TEXTURE_Y + BUTTON_HEIGHT);
			glVertex2f(_x + BUTTON_WIDTH, _y);

			glTexCoord2f(BUTTON_RED_OUT_TEXTURE_X,                BUTTON_RED_OUT_TEXTURE_Y + BUTTON_HEIGHT);
			glVertex2f(_x,                _y );
		}
	}

	glTexCoord2f(_textTextureX,                             _textTextureY);
	glVertex2f(_x + BUTTON_TO_TEXT_OFFSET_X,                             _y + BUTTON_TO_TEXT_OFFSET_Y + BUTTON_TEXT_TEXTURE_HEIGHT);

	glTexCoord2f(_textTextureX + BUTTON_TEXT_TEXTURE_WIDTH, _textTextureY);
	glVertex2f(_x + BUTTON_TO_TEXT_OFFSET_X + BUTTON_TEXT_TEXTURE_WIDTH, _y + BUTTON_TO_TEXT_OFFSET_Y + BUTTON_TEXT_TEXTURE_HEIGHT);

	glTexCoord2f(_textTextureX + BUTTON_TEXT_TEXTURE_WIDTH, _textTextureY + BUTTON_TEXT_TEXTURE_HEIGHT);
	glVertex2f(_x + BUTTON_TO_TEXT_OFFSET_X + BUTTON_TEXT_TEXTURE_WIDTH, _y + BUTTON_TO_TEXT_OFFSET_Y);

	glTexCoord2f(_textTextureX,                             _textTextureY + BUTTON_TEXT_TEXTURE_HEIGHT);
	glVertex2f(_x + BUTTON_TO_TEXT_OFFSET_X,                             _y + BUTTON_TO_TEXT_OFFSET_Y);

	glEnd();
	MenuManager::getMenuTexture()->setActive(false);
}
GRAPHICSLAB_NAMESPACE_END
