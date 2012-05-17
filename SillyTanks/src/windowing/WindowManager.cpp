/**
 * windowManager.cpp
 * This class handles all the keyboard and mouse inputs that are handled by glut
 */

// Class declaration include
#include "WindowManager.hpp"

// common includes
#include "../common/GLIncludes.hpp"
#include "../common/Exception.hpp"

namespace game_space {

//////////////////////////////////////////////////////////////////////////////
// Global variable and functions for binding GLUT callbacks
//////////////////////////////////////////////////////////////////////////////

static void __displayFunc()
{
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if ( window == NULL )
	{
		throw Exception( "Active window hasn't been set on WindowManager" );
	}

	window->onPaint();
}

static void __reshapeFunc( int width, int height )
{
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if ( window == NULL )
	{
		throw Exception( "Active window hasn't been set on WindowManager" );
	}

	window->onResize( width, height );
}

static void __entryFunc( int state )
{
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if ( window == NULL )
	{
		throw Exception( "Active window hasn't been set on WindowManager" );
	}

	window->onMouseEntry( state );
}

static void __mouseFunc( int button, int state, int x, int y )
{
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if ( window == NULL )
	{
		throw Exception( "Active window hasn't been set on WindowManager" );
	}

	window->onMouseClick( button, state, x, y );
}

static void __motionFunc( int x, int y )
{
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if ( window == NULL )
	{
		throw Exception( "Active window hasn't been set on WindowManager" );
	}

	window->onMouseMove( x, y );
}

static void __passiveMotionFunc( int x, int y )
{
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if ( window == NULL )
	{
		throw Exception( "Active window hasn't been set on WindowManager" );
	}

	window->onMousePassiveMove( x, y );
}

static void __visibilityFunc( int state )
{
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if ( window == NULL )
	{
		throw Exception( "Active window hasn't been set on WindowManager" );
	}

	window->onVisible( state );
}

static void __timerFunc( int value )
{
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if ( window == NULL )
	{
		throw Exception( "Active window hasn't been set on WindowManager" );
	}

	window->onTimer( value );

	// To have max 30 frames per second,
	// we schedule to run this function every 1000/33'th millisecond.
	static const int MAX_FPS = 30;
	static const int MILLIS_PER_FRAME = 1000/MAX_FPS;
	glutTimerFunc( MILLIS_PER_FRAME, __timerFunc, MILLIS_PER_FRAME );
}

static void __idleFunc()
{
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if ( window == NULL )
	{
		throw Exception( "Active window hasn't been set on WindowManager" );
	}

	window->onIdle();
}

static void __keyPressed (unsigned char key, int x, int y) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if ( window == NULL )
	{
		throw Exception( "Active window hasn't been set on WindowManager" );
	}
	window->setKey(key,true);
}

static void __keyUp (unsigned char key, int x, int y) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if ( window == NULL )
	{
		throw Exception( "Active window hasn't been set on WindowManager" );
	}
	window->setKey(key,false);
}

static void __specialKeyPressed (int key, int x, int y) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if ( window == NULL )
	{
		throw Exception( "Active window hasn't been set on WindowManager" );
	}
	window->setSpecialKey(key,true);
}

static void __specialKeyUp (int key, int x, int y) {
	WindowManager &windowManager = WindowManager::getInstance();
	Window *window = windowManager.getActiveWindow();
	if ( window == NULL )
	{
		throw Exception( "Active window hasn't been set on WindowManager" );
	}
	window->setSpecialKey(key,false);
}

//////////////////////////////////////////////////////////////////////////////

WindowManager *WindowManager::_instance = NULL;

WindowManager &WindowManager::getInstance()
{
	if ( _instance == NULL )
	{
		_instance = new WindowManager();
	}

	return ( *_instance );
}

WindowManager::WindowManager() :
_activeWindow( NULL )
{
}

WindowManager::~WindowManager()
{
}

Window *WindowManager::createWindow( Window::Parameters &parameters )
{
	Window *window = new Window( parameters );

	glutInitWindowSize( parameters.width, parameters.height );
	glutInitWindowPosition( parameters.posX, parameters.posY );

	// Create GLUT Window
	int windowID = glutCreateWindow( parameters.title.c_str() );
	window->setID( windowID );

	_activeWindow = window;

#ifdef WIN32
	// Initialize GLEW
	if ( GLEW_OK != glewInit() )
	{
		throw Exception( std::string( "Failed to initialize GLEW library" ) );
	}

	if ( !glewIsSupported( "GL_ARB_texture_rectangle" ) )
	{
		throw Exception( std::string( "GL_TEXTURE_RECTANGLE_ARB extension supported" ) );
	}
#endif

	// Bind handlers
	glutDisplayFunc( __displayFunc );
	glutReshapeFunc( __reshapeFunc );
	glutMouseFunc( __mouseFunc );
	glutMotionFunc( __motionFunc );
	glutPassiveMotionFunc( __passiveMotionFunc );
	glutEntryFunc( __entryFunc );
	glutVisibilityFunc( __visibilityFunc );
	glutTimerFunc( 1, __timerFunc, 1 );

	//get keyboard inputs to check if multiple keys are pressed simultaneously
	glutKeyboardFunc(__keyPressed);// Tell GLUT to use the method "keyPressed" for key presses
	glutKeyboardUpFunc(__keyUp);// Tell GLUT to use the method "keyUp" for key up events

	glutSpecialFunc(__specialKeyPressed);// Tell GLUT to use the method "keySpecial" for special key presses
	glutSpecialUpFunc(__specialKeyUp);// Tell GLUT to use the method "keySpecialUp" for special up key events

	window->initialize();
	return window;
}

void WindowManager::deleteWindow( Window &window )
{
	glutDestroyWindow( window.getID() );
	delete ( &window );
}

}
