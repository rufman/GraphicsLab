/**
 * window.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "window.hpp"

// Includes
#include "gl_includes.hpp"
#include "windowManager.hpp"
#include "scene.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN	

Window::Window( const Parameters &parameters ) :
    _parameters( parameters )
{
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

void Window::onKey( unsigned char key )
{
    int modifier = glutGetModifiers();
    bool shiftPressed = ( modifier & GLUT_ACTIVE_SHIFT );
    bool ctrlPressed = ( modifier & GLUT_ACTIVE_CTRL );
    bool altPressed = ( modifier & GLUT_ACTIVE_ALT );
    
    switch ( key )
    {
        case 13:
        {
            if(!altPressed) {
            	break;
            }
        	_parameters.fullScreen = !_parameters.fullScreen;
            if ( _parameters.fullScreen ) {
            	fullscreenOn();
            } else {
            	fullscreenOff();
            }
            
            break;
        }
        default:
        {
            _scene->onKey( key );
            break;
        }
    }
}

void Window::onKeyUp( unsigned char key )
{
    _scene->onKeyUp( key );
}

void Window::onSpecialKey( int key )
{
    _scene->onSpecialKey( key );
}

void Window::onSpecialKeyUp( int key )
{
    _scene->onSpecialKeyUp( key );
}

void Window::onMouseEntry( int state )
{
    _scene->onMouseEntry( state );
}

void Window::onMouseClick( int button, int state, int x, int y )
{
    _scene->onMouseClick( button, state, x, y );
}

void Window::onMouseMove( int x, int y )
{
    _scene->onMouseMove( x, y );
}

void Window::onMousePassiveMove( int x, int y )
{
    _scene->onMousePassiveMove( x, y );
}

void Window::onVisible( int state )
{
    _scene->onVisible( state );
}

void Window::onTimer( int value )
{
    _scene->onTimer( value );
}

void Window::onIdle()
{
    _scene->onIdle();
}

void Window::fullscreenOn() {
	_parameters.fullScreen = true;
	GraphicsSettings::setFullscreen(1);

    // Remember window position and size
    _parameters.posX = glutGet( GLUT_WINDOW_X );
    _parameters.posY = glutGet( GLUT_WINDOW_Y );
    _parameters.width = glutGet( GLUT_WINDOW_WIDTH );
    _parameters.height = glutGet( GLUT_WINDOW_HEIGHT );

    // Switch to fullscreen mode
    glutFullScreen();
}

void Window::fullscreenOff() {
	_parameters.fullScreen = false;
	GraphicsSettings::setFullscreen(0);

    // Position and size window to previous values
    glutPositionWindow( _parameters.posX, _parameters.posY );
    glutReshapeWindow( _parameters.width, _parameters.height );
}

GRAPHICSLAB_NAMESPACE_END



