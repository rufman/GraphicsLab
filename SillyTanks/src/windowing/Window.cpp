/**
 * window.cpp
* This is the setup for our window. It handles some of the keypresses that do not have to be handled by the graphics.
 */

// Class declaration include
#include "Window.hpp"

// Includes
#include "../common/GLIncludes.hpp"
#include "WindowManager.hpp"
#include "../scene/Scene.hpp"

#include <cstdlib>

GAME_NAMESPACE_BEGIN

Window::Window( const Parameters &parameters ) :
_parameters( parameters )
{
	//nothing is pressed at startup
	for(int i = 0; i < 256;i++)
	{
		_keyPressed[i] = false;
	}

	_scene = new Scene( *this );
}

Window::~Window()
{
	delete _scene;
}

void Window::initialize()
{
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
	_scene->onResize( width, height );
	glutPostRedisplay();
}

void Window::setKey(unsigned char key,bool value) {
	_keyPressed[key] = value;

	if(_keyPressed['q'] || _keyPressed['Q'] || _keyPressed[27] )
	{
		exit( EXIT_SUCCESS );
	}

	if( _keyPressed['f'] || _keyPressed['F'])
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
	_scene->setKey(key,value);
}

void Window::setSpecialKey(int key, bool value) {
	_scene->setSpecialKey(key, value);
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

GAME_NAMESPACE_END
