/*
 * input.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "inputManager.hpp"

// Includes
#include "gl_includes.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

int InputManager::_mouseX;
int InputManager::_mouseY;
int InputManager::_mouseDeltaXtemp;
int InputManager::_mouseDeltaYtemp;
int InputManager::_mouseDeltaX;
int InputManager::_mouseDeltaY;

bool* InputManager::_keyUp;
bool* InputManager::_keyUpTemp;
bool* InputManager::_keyDown;
bool* InputManager::_keyDownTemp;
bool* InputManager::_keyHeld;

InputManager::InputManager() {
	InputManager::_mouseX = 0;
	InputManager::_mouseY = 0;
	InputManager::_mouseDeltaXtemp = 0;
	InputManager::_mouseDeltaYtemp = 0;
	InputManager::_mouseDeltaX = 0;
	InputManager::_mouseDeltaY = 0;

	InputManager::_keyUp = new bool[387];
	InputManager::_keyUpTemp = new bool[387];
	InputManager::_keyDown = new bool[387];
	InputManager::_keyDownTemp = new bool[387];
	InputManager::_keyHeld = new bool[387];

	for(int i = 0; i < 387; i++) {
		InputManager::_keyUp[i] = false;
		InputManager::_keyUpTemp[i] = false;
		InputManager::_keyDown[i] = false;
		InputManager::_keyDownTemp[i] = false;
		InputManager::_keyHeld[i] = false;
	}
}


void InputManager::update() {
	for(int i = 0; i < 387; i++) {
		if(InputManager::_keyUp[i]) {
			InputManager::_keyUp[i] = false;
		}
		if(InputManager::_keyUpTemp[i]) {
			InputManager::_keyUpTemp[i] = false;
			InputManager::_keyUp[i] = true;
		}
		if(InputManager::_keyDown[i]) {
			InputManager::_keyDown[i] = false;
		}
		if(InputManager::_keyDownTemp[i]) {
			InputManager::_keyDownTemp[i] = false;
			InputManager::_keyDown[i] = true;
		}
	}

	_mouseDeltaX = _mouseDeltaXtemp;
	_mouseDeltaXtemp = 0;

	_mouseDeltaY = _mouseDeltaYtemp;
	_mouseDeltaYtemp = 0;

}


void InputManager::onKey(unsigned char key) {
	_keyHeld[key] = true;
	_keyDownTemp[key] = true;
}

void InputManager::onKeyUp(unsigned char key) {
	_keyHeld[key] = false;
	_keyUpTemp[key] = true;
}

void InputManager::onSpecialKey(int key) {
	key += 256;
	_keyHeld[key] = true;
	_keyDownTemp[key] = true;
}

void InputManager::onSpecialKeyUp(int key) {
	key += 256;
	_keyHeld[key] = false;
	_keyUpTemp[key] = true;
}

bool InputManager::keyUp(unsigned char key) {
	return _keyUp[key];
}

bool InputManager::keyDown(unsigned char key) {
	return _keyDown[key];
}

bool InputManager::keyHeld(unsigned char key) {
	return _keyHeld[key];
}

bool InputManager::specialKeyUp(int key) {
	return _keyUp[key + 256];
}

bool InputManager::specialKeyDown(int key) {
	return _keyDown[key + 256];
}

bool InputManager::specialKeyHeld(int key) {
	return _keyHeld[key + 256];
}

void InputManager::onMouseMove(int x, int y) {
	_mouseDeltaXtemp += x - _mouseX;
	_mouseDeltaYtemp += y - _mouseY;
	_mouseX = x;
	_mouseY = y;
}

void InputManager::onMouseClick(int button, int state) {

	int index;
	if(button == GLUT_LEFT_BUTTON) {
		index = 384;
	} else if(button == GLUT_MIDDLE_BUTTON) {
		index = 385;
	} else if(button == GLUT_RIGHT_BUTTON) {
		index = 386;
	}

	if(state == GLUT_DOWN) {
		_keyHeld[index] = true;
		_keyDownTemp[index] = true;
	} else {
		_keyHeld[index] = false;
		_keyUpTemp[index] = true;
	}
}

bool InputManager::mouseUp(int key) {
	return _keyUp[key + 384];
}

bool InputManager::mouseDown(int key) {
	return _keyDown[key + 384];
}

bool InputManager::mouseHeld(int key) {
	return _keyHeld[key + 384];
}

int InputManager::mouseX() {
	return _mouseX;
}

int InputManager::mouseY() {
	return _mouseY;
}

int InputManager::mouseDeltaX() {
	return _mouseDeltaX;
}

int InputManager::mouseDeltaY() {
	return _mouseDeltaY;
}

void InputManager::moveMouse(int x, int y) {
	glutWarpPointer(x, y);
}

void InputManager::stickyMouse() {
#ifdef WIN32
	_mouseX -= _mouseDeltaX;
	_mouseY -= _mouseDeltaY;
	glutWarpPointer(_mouseX, _mouseY);
#endif
}

GRAPHICSLAB_NAMESPACE_END

