/**
 * window.cpp
 * This is the setup for our window. It handles some of the key presses that do not have to be handled by the graphics.
 */

// Class declaration include
#include "Window.hpp"

//windowing includes
#include "WindowManager.hpp"

// common includes
#include "../common/GLIncludes.hpp"

//scene includes
#include "../scene/Scene.hpp"

//std includes
#include <cstdlib>

namespace game_space {

Window::Window( const Parameters &parameters ) :
_parameters( parameters )
{
	//set all the keys as not pressed
	resetKeys();

	//instatiate a new scene
	_scene = new Scene( *this );
}

Window::~Window()
{
	delete _scene;
}

void Window::initialize()
{
	//initialize the new scene
	_scene->initialize();
}

void Window::setID( int windowID )
{
	_windowID = windowID;
}

void Window::onPaint()
{
	_scene->onPaint();
}

void Window::onResize( int width, int height )
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (height == 0)
		height = 1;

	float ratio = 1.0 * width / height;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	// Set the correct perspective.
	gluPerspective(45, ratio, 1, 100);
	glMatrixMode(GL_MODELVIEW);

	_scene->onResize( width, height );
	glutPostRedisplay();
}

void Window::resetKeys()
{
	//nothing is pressed at startup
	for(int i = 0; i < 256;i++)
	{
		_keyPressed[i] = false;
		_oldKeyPressed[i] = false;
		_keyHit[i] = false;
	}

	for(int i = 0;i < 246;i++)
	{
		_specialKeyPressed[i] = false;
		_oldSpecialKeyPressed[i] = false;
		_specialKeyHit[i] = false;
	}
}

bool Window::keyPressed(char key)
{
	return _keyPressed[(int)key];
}

bool Window::keyHit(char key)
{
	bool keyHit = _keyHit[(int)key];
	_keyHit[(int)key] = false;
	return keyHit;
}

bool Window::specialKeyPressed(int key)
{
	return _specialKeyPressed[key];
}

bool Window::specialKeyHit(int key)
{
	bool keyHit = _specialKeyHit[key];
	_specialKeyHit[key] = false;
	return keyHit;
}

void Window::setKey(unsigned char key,bool value) {
	_oldKeyPressed[key] = _keyPressed[key];
	_keyPressed[key] = value;
	_keyHit[key] = (_oldKeyPressed[key] && !_keyPressed[key]);

	if(keyPressed(27) )
	{
		exit( EXIT_SUCCESS );
	}

	if( keyPressed('f') || keyPressed('F'))
	{
		_parameters.fullScreen = !_parameters.fullScreen;
		if ( _parameters.fullScreen )
		{
			// Remember window position and size
			_parameters.posX = glutGet( GLUT_WINDOW_X );
			_parameters.posY = glutGet( GLUT_WINDOW_Y );
			_parameters.width = glutGet( GLUT_WINDOW_WIDTH );
			_parameters.height = glutGet( GLUT_WINDOW_HEIGHT );

			// Switch to fullscreen mode
			glutFullScreen();
		}
		else
		{
			// Position and size window to previous values
			glutPositionWindow( _parameters.posX, _parameters.posY );
			glutReshapeWindow( _parameters.width, _parameters.height );
		}
	}
}

void Window::setSpecialKey(int key, bool value) {
	_oldSpecialKeyPressed[key] = _specialKeyPressed[key];
	_specialKeyPressed[key] = value;
	_specialKeyHit[key] = (_oldSpecialKeyPressed[key] && !_specialKeyPressed[key]);
}

void Window::onMouseEntry(int state) {
	_scene->onMouseEntry(state);
}

void Window::onMouseClick(int button, int state, int x, int y) {
	_scene->onMouseClick(button, state, x, y);
}

void Window::onMouseMove(int x, int y) {
	_scene->onMouseMove(x, y);
}

void Window::onMousePassiveMove(int x, int y) {
	_scene->onMousePassiveMove(x, y);
}

void Window::onVisible(int state) {
	_scene->onVisible(state);
}

void Window::onTimer(int value) {
	_scene->onTimer(value);
}

void Window::onIdle() {
	_scene->onIdle();
}

}
