/*
 * menu.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "menu.hpp"

#include "inputManager.hpp"
#include "gl_includes.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN
Menu::Menu(Scene &scene_, MenuType type_) :
	Drawable(scene_,0,0,0),
	_type(type_),
	_highLightedButton(NULL) {
	updateOffsets();
}

Menu::~Menu() {
	for(int i = 0; i < _buttonList.size(); i++) {
		delete _buttonList.at(i);
	}
}


void Menu::update(){
	
	if (_highLightedButton != NULL && !_highLightedButton->isMouseInside(this)) {
		_highLightedButton->rollOut();
		_highLightedButton = NULL;
	}
	
	for (int i = 0; i < _buttonList.size(); i++) {
		if (_buttonList.at(i)->isMouseInside(this)) {
			if(_highLightedButton != _buttonList.at(i)) {
				_buttonList.at(i)->rollOver();
				_highLightedButton = _buttonList.at(i);
			}
			
			if (InputManager::mouseDown(GLUT_LEFT_BUTTON)) {
				_buttonList.at(i)->onClick();
			}
		}
	}
}


void Menu::addButton(Button *button_) {
	_buttonList.push_back(button_);
}


void Menu::draw() {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	// Draw background overlay
	if(!MenuManager::isInIntroMenuMode()) {
		glColor4f(0.0, 0.0, 0.0, MENU_OVERLAY_ALPHA);
		glBegin(GL_QUADS);
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, height);
		glVertex2f(width, height);
		glVertex2f(width, 0.0);
		glEnd();
	}

	updateOffsets();
	glTranslatef(_offsetX, _offsetY, 0);

	MenuManager::getMenuTexture()->setActive(true);

	// Draw the outer frame box
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBegin(GL_QUADS);

	glTexCoord2f(MENU_TEXTURE_X, MENU_TEXTURE_Y);
	glVertex2f(0.0, MENU_TEXTURE_HEIGHT);

	glTexCoord2f(MENU_TEXTURE_X, MENU_TEXTURE_Y + MENU_TEXTURE_HEIGHT);
	glVertex2f(0.0, 0.0);

	glTexCoord2f(MENU_TEXTURE_X + MENU_TEXTURE_WIDTH, MENU_TEXTURE_Y + MENU_TEXTURE_HEIGHT);
	glVertex2f(MENU_TEXTURE_WIDTH, 0.0);

	glTexCoord2f(MENU_TEXTURE_X + MENU_TEXTURE_WIDTH, MENU_TEXTURE_Y);
	glVertex2f(MENU_TEXTURE_WIDTH, MENU_TEXTURE_HEIGHT);

	if(_type == MAIN_MENU) {
		glTexCoord2f(MENU_LOGO_TEXTURE_X, MENU_LOGO_TEXTURE_Y);
		glVertex2f(MENU_LOGO_X, MENU_LOGO_Y + MENU_LOGO_TEXTURE_HEIGHT);

		glTexCoord2f(MENU_LOGO_TEXTURE_X, MENU_LOGO_TEXTURE_Y + MENU_LOGO_TEXTURE_HEIGHT);
		glVertex2f(MENU_LOGO_X, MENU_LOGO_Y);

		glTexCoord2f(MENU_LOGO_TEXTURE_X + MENU_LOGO_TEXTURE_WIDTH, MENU_LOGO_TEXTURE_Y + MENU_LOGO_TEXTURE_HEIGHT);
		glVertex2f(MENU_LOGO_X + MENU_LOGO_TEXTURE_WIDTH, MENU_LOGO_Y);

		glTexCoord2f(MENU_LOGO_TEXTURE_X + MENU_LOGO_TEXTURE_WIDTH, MENU_LOGO_TEXTURE_Y);
		glVertex2f(MENU_LOGO_X + MENU_LOGO_TEXTURE_WIDTH, MENU_LOGO_Y + MENU_LOGO_TEXTURE_HEIGHT);
	}

	if(_type == LOAD_MENU) {
		glTexCoord2f(MENU_TITLE_TEXTURE_X, MENU_TITLE_LOAD_GAME_TEXTURE_Y);
		glVertex2f(MENU_TITLE_X, MENU_TITLE_Y + MENU_TITLE_TEXTURE_HEIGHT);

		glTexCoord2f(MENU_TITLE_TEXTURE_X, MENU_TITLE_LOAD_GAME_TEXTURE_Y + MENU_TITLE_TEXTURE_HEIGHT);
		glVertex2f(MENU_TITLE_X, MENU_TITLE_Y);

		glTexCoord2f(MENU_TITLE_TEXTURE_X + MENU_TITLE_TEXTURE_WIDTH, MENU_TITLE_LOAD_GAME_TEXTURE_Y + MENU_TITLE_TEXTURE_HEIGHT);
		glVertex2f(MENU_TITLE_X + MENU_TITLE_TEXTURE_WIDTH, MENU_TITLE_Y);

		glTexCoord2f(MENU_TITLE_TEXTURE_X + MENU_TITLE_TEXTURE_WIDTH, MENU_TITLE_LOAD_GAME_TEXTURE_Y);
		glVertex2f(MENU_TITLE_X + MENU_TITLE_TEXTURE_WIDTH, MENU_TITLE_Y + MENU_TITLE_TEXTURE_HEIGHT);
	}

	if(_type == SAVE_MENU) {
		glTexCoord2f(MENU_TITLE_TEXTURE_X, MENU_TITLE_SAVE_GAME_TEXTURE_Y);
		glVertex2f(MENU_TITLE_X, MENU_TITLE_Y + MENU_TITLE_TEXTURE_HEIGHT);

		glTexCoord2f(MENU_TITLE_TEXTURE_X, MENU_TITLE_SAVE_GAME_TEXTURE_Y + MENU_TITLE_TEXTURE_HEIGHT);
		glVertex2f(MENU_TITLE_X, MENU_TITLE_Y);

		glTexCoord2f(MENU_TITLE_TEXTURE_X + MENU_TITLE_TEXTURE_WIDTH, MENU_TITLE_SAVE_GAME_TEXTURE_Y + MENU_TITLE_TEXTURE_HEIGHT);
		glVertex2f(MENU_TITLE_X + MENU_TITLE_TEXTURE_WIDTH, MENU_TITLE_Y);

		glTexCoord2f(MENU_TITLE_TEXTURE_X + MENU_TITLE_TEXTURE_WIDTH, MENU_TITLE_SAVE_GAME_TEXTURE_Y);
		glVertex2f(MENU_TITLE_X + MENU_TITLE_TEXTURE_WIDTH, MENU_TITLE_Y + MENU_TITLE_TEXTURE_HEIGHT);
	}

	if(_type == SETTINGS_MENU) {
		glTexCoord2f(MENU_TITLE_TEXTURE_X, MENU_TITLE_SETTINGS_TEXTURE_Y);
		glVertex2f(MENU_TITLE_X, MENU_TITLE_Y + MENU_TITLE_TEXTURE_HEIGHT);

		glTexCoord2f(MENU_TITLE_TEXTURE_X, MENU_TITLE_SETTINGS_TEXTURE_Y + MENU_TITLE_TEXTURE_HEIGHT);
		glVertex2f(MENU_TITLE_X, MENU_TITLE_Y);

		glTexCoord2f(MENU_TITLE_TEXTURE_X + MENU_TITLE_TEXTURE_WIDTH, MENU_TITLE_SETTINGS_TEXTURE_Y + MENU_TITLE_TEXTURE_HEIGHT);
		glVertex2f(MENU_TITLE_X + MENU_TITLE_TEXTURE_WIDTH, MENU_TITLE_Y);

		glTexCoord2f(MENU_TITLE_TEXTURE_X + MENU_TITLE_TEXTURE_WIDTH, MENU_TITLE_SETTINGS_TEXTURE_Y);
		glVertex2f(MENU_TITLE_X + MENU_TITLE_TEXTURE_WIDTH, MENU_TITLE_Y + MENU_TITLE_TEXTURE_HEIGHT);
	}

	glEnd();

	MenuManager::getMenuTexture()->setActive(false);

	//Draw Components
	drawButtons();
	glPopMatrix();
}


void Menu::drawButtons() {
	for(int i = 0; i < _buttonList.size(); i++) {
		_buttonList.at(i)->draw();
	}
}

void Menu::updateOffsets() {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	_offsetX = (width / 2) - (MENU_TEXTURE_WIDTH / 2);
	_offsetY = (height / 2) - (MENU_TEXTURE_HEIGHT / 2);
}

int Menu::getOffsetX() {
	return _offsetX;
}

int Menu::getOffsetY() {
	return _offsetY;
}

GRAPHICSLAB_NAMESPACE_END

