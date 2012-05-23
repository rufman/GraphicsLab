/**
 * windowManager.cpp
 * This class handles all the keyboard and mouse inputs that are handled by glut
 */

// Class declaration include
#include "WindowManager.hpp"

// common includes
#include "../common/GLIncludes.hpp"
#include "../common/Exception.hpp"

//std includes
#include <iostream>

namespace game_space {

//////////////////////////////////////////////////////////////////////////////
// Global variable and functions for binding GLUT callbacks
//////////////////////////////////////////////////////////////////////////////

static void __displayFunc() {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if (window == NULL) {
		throw Exception("Active window hasn't been set on WindowManager");
	}

	window->onPaint();
}

static void __reshapeFunc(int width, int height) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if (window == NULL) {
		throw Exception("Active window hasn't been set on WindowManager");
	}

	window->onResize(width, height);
}

static void __entryFunc(int state) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if (window == NULL) {
		throw Exception("Active window hasn't been set on WindowManager");
	}

	window->onMouseEntry(state);
}

static void __mouseFunc(int button, int state, int x, int y) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if (window == NULL) {
		throw Exception("Active window hasn't been set on WindowManager");
	}

	window->onMouseClick(button, state, x, y);
}

static void __motionFunc(int x, int y) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if (window == NULL) {
		throw Exception("Active window hasn't been set on WindowManager");
	}

	window->onMouseMove(x, y);
}

static void __passiveMotionFunc(int x, int y) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if (window == NULL) {
		throw Exception("Active window hasn't been set on WindowManager");
	}

	window->onMousePassiveMove(x, y);
}

static void __visibilityFunc(int state) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if (window == NULL) {
		throw Exception("Active window hasn't been set on WindowManager");
	}

	window->onVisible(state);
}

static void __timerFunc(int value) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if (window == NULL) {
		throw Exception("Active window hasn't been set on WindowManager");
	}

	window->onTimer(value);

	// To have max 30 frames per second,
	// we schedule to run this function every 1000/33'th millisecond.
	static const int MAX_FPS = 30;
	static const int MILLIS_PER_FRAME = 1000 / MAX_FPS;
	glutTimerFunc(MILLIS_PER_FRAME, __timerFunc, MILLIS_PER_FRAME);
}

static void __keyPressed(unsigned char key, int x, int y) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if (window == NULL) {
		throw Exception("Active window hasn't been set on WindowManager");
	}
	window->setKey(key, true);
}

static void __keyUp(unsigned char key, int x, int y) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if (window == NULL) {
		throw Exception("Active window hasn't been set on WindowManager");
	}
	window->setKey(key, false);
}

static void __specialKeyPressed(int key, int x, int y) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if (window == NULL) {
		throw Exception("Active window hasn't been set on WindowManager");
	}
	window->setSpecialKey(key, true);
}

static void __specialKeyUp(int key, int x, int y) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if (window == NULL) {
		throw Exception("Active window hasn't been set on WindowManager");
	}
	window->setSpecialKey(key, false);
}

//////////////////////////////////////////////////////////////////////////////

WindowManager *WindowManager::_instance = NULL;

WindowManager &WindowManager::getInstance() {
	if (_instance == NULL) {
		_instance = new WindowManager();
	}

	return (*_instance);
}

WindowManager::WindowManager() :
		_activeWindow(NULL) {
}

WindowManager::~WindowManager() {
}

Window *WindowManager::createWindow(Window::Parameters &parameters) {
	// Initialize GLUT
	glutInit(&parameters.argc, parameters.argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	std::cout << "WindowManager initialized glut.\n";

	glutInitWindowPosition(parameters.posX, parameters.posY);
	glutInitWindowSize(parameters.width, parameters.height);
	// Create GLUT Window
	int windowID = glutCreateWindow(parameters.title.c_str());

	// Bind handlers
	glutDisplayFunc(__displayFunc);
	glutIdleFunc(__displayFunc);
	glutReshapeFunc(__reshapeFunc);
	glutMouseFunc(__mouseFunc);
	glutMotionFunc(__motionFunc);
	glutPassiveMotionFunc(__passiveMotionFunc);
	glutEntryFunc(__entryFunc);
	glutVisibilityFunc(__visibilityFunc);
	glutTimerFunc(1, __timerFunc, 1);

	//get keyboard inputs to check if multiple keys are pressed simultaneously
	glutKeyboardFunc(__keyPressed); // Tell GLUT to use the method "keyPressed" for key presses
	glutKeyboardUpFunc(__keyUp); // Tell GLUT to use the method "keyUp" for key up events

	glutSpecialFunc(__specialKeyPressed); // Tell GLUT to use the method "specialKeyPressed" for special key presses
	glutSpecialUpFunc(__specialKeyUp); // Tell GLUT to use the method "specialKeyUp" for special up key events

	glEnable(GL_DEPTH_TEST);

	// Initialize GLEW
	if (GLEW_OK != glewInit()) {
		throw Exception(std::string("Failed to initialize GLEW library"));
	}
	else
	{
		std::cout << "WindowManager initialized glew.\n";
	}

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		{
			std::cout << "WindowManager says: GLSL support.\n";
		}
		else {
			std::cout << "WindowManager says: No GLSL support.\n";
			exit(1);
		}

	Window *window = new Window(parameters);
	window->setID(windowID);

	_activeWindow = window;

	window->initialize();
	return window;
}

void WindowManager::deleteWindow(Window &window) {
	glutDestroyWindow(window.getID());
	delete (&window);
}

}
