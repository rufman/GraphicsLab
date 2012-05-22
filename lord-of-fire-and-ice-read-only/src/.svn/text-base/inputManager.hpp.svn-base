/*
 * input.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef INPUTMANAGER_HPP_
#define INPUTMANAGER_HPP_

// Includes
#include "defs.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN


class InputManager {

public:

	InputManager();

	void update();

	static void onKey(unsigned char key);

	static void onKeyUp(unsigned char key);

	static bool keyUp(unsigned char key);

	static bool keyDown(unsigned char key);

	static bool keyHeld(unsigned char key);


	static void onSpecialKey(int key);

	static void onSpecialKeyUp(int key);

	static bool specialKeyUp(int key);

	static bool specialKeyDown(int key);

	static bool specialKeyHeld(int key);


	static void onMouseMove(int x, int y);

	static void onMouseClick(int button, int state);

	static bool mouseUp(int key);

	static bool mouseDown(int key);

	static bool mouseHeld(int key);

	static int mouseX();

	static int mouseY();

	static int mouseDeltaX();

	static int mouseDeltaY();


	static void moveMouse(int x, int y);

	static void stickyMouse();



private:

	static int _mouseX, _mouseY;
	static int _mouseDeltaXtemp, _mouseDeltaYtemp, _mouseDeltaX, _mouseDeltaY;

	static bool* _keyUp;
	static bool* _keyUpTemp;
	static bool* _keyDown;
	static bool* _keyDownTemp;
	static bool* _keyHeld;

};


GRAPHICSLAB_NAMESPACE_END

#endif /* INPUTMANAGER_HPP_ */
