/**
 * application.cpp
 * This class parses the arguments, initializes glut and glew and the window used for drawing and runs the glut main loop.
 */
// Class declaration include
#include "Application.hpp"

//windowing includes
#include "../windowing/WindowManager.hpp"

// common includes
#include "../common/Exception.hpp"
#include "../common/GLIncludes.hpp"

// std includes
#include <iostream>
#include <stdio.h>

namespace game_space {

Application *Application::_instance = NULL;

Application &Application::getInstance() {
	if (_instance == NULL) {
		_instance = new Application();
	}

	return (*_instance);
}

Application::Application() :
		_mainWindow(NULL) {
}

Application::~Application() {
	WindowManager &windowManager = WindowManager::getInstance();
	windowManager.deleteWindow(*_mainWindow);
	_mainWindow = NULL;
}

Application::Parameters::Parameters() :
		//set the window title to GUI TITLE
		windowTitle(std::string(WINDOW_TITLE)) {
}

void Application::Parameters::parse(int argc, char **argv) {
	//default fog params
	fogRed = 0.8;
	fogGreen = 0.8;
	fogBlue = 0.8;
	fogDensity = 0.025;
	fogStart = 1;
	fogEnd = 20;

	// Parse arguments (they will be set later on by the menu)
	for (int argID = 1; argID < argc;) {
		std::string arg = argv[argID++];
		if (arg == "-s" || arg == "-sky") {
			skyTextureFile = argv[argID++];
		} else if (arg == "-t" || arg == "-terrain") {
			terrainFilePrefix = argv[argID++];
		} else if (arg == "-w" || arg == "-water") {
			waterHeight = atof(argv[argID++]);
		} else if (arg == "-fd" || arg == "-fogdensity") {
			fogDensity = atof(argv[argID++]);
		} else if (arg == "-fs" || arg == "-fogstart") {
			fogStart = atof(argv[argID++]);
		} else if (arg == "-fe" || arg == "-fogend") {
			fogEnd = atof(argv[argID++]);
		} else if (arg == "-fr" || arg == "-fogr") {
			fogRed = atof(argv[argID++]);
		} else if (arg == "-fg" || arg == "-fogg") {
			fogGreen = atof(argv[argID++]);
		} else if (arg == "-fb" || arg == "-fogb") {
			fogBlue = atof(argv[argID++]);
		} else {
			std::cerr << "WARNING: Unknown application parameter \"" << arg
					<< "\"" << std::endl;
		}
	}

}

void Application::initialize(int argc, char **argv) {
	WindowManager &windowManager = WindowManager::getInstance();
	if (_mainWindow) {
		windowManager.deleteWindow(*_mainWindow);
		_mainWindow = NULL;
	}

	_parameters.parse(argc, argv);

	Window::Parameters windowParameters;
	windowParameters.title = _parameters.windowTitle;
	windowParameters.argc = argc;
	windowParameters.argv = argv;
	_mainWindow = windowManager.createWindow(windowParameters);
}

int Application::run() {
	glutMainLoop();
	return EXIT_SUCCESS;
}

}
