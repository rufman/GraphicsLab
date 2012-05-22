/**
 * windowManager.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "windowManager.hpp"

// Includes
#include "gl_includes.hpp"
#include "exception.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN	

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

static void __keyboardFunc( unsigned char key, int x, int y )
{
    WindowManager &windowManager = WindowManager::getInstance();
    Window *window = windowManager.getActiveWindow();
    if ( window == NULL )
    {
        throw Exception( "Active window hasn't been set on WindowManager" );
    }
    
    window->onKey( key );    
}

static void __keyboardUpFunc( unsigned char key, int x, int y )
{
    WindowManager &windowManager = WindowManager::getInstance();
    Window *window = windowManager.getActiveWindow();
    if ( window == NULL )
    {
        throw Exception( "Active window hasn't been set on WindowManager" );
    }

    window->onKeyUp( key );
}

static void __specialFunc( int key, int x, int y )
{    
    WindowManager &windowManager = WindowManager::getInstance();
    Window *window = windowManager.getActiveWindow();
    if ( window == NULL )
    {
        throw Exception( "Active window hasn't been set on WindowManager" );
    }
    
    window->onSpecialKey( key );
}

static void __specialUpFunc( int key, int x, int y )
{
    WindowManager &windowManager = WindowManager::getInstance();
    Window *window = windowManager.getActiveWindow();
    if ( window == NULL )
    {
        throw Exception( "Active window hasn't been set on WindowManager" );
    }

    window->onSpecialKeyUp( key );
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
    
    // Bind handlers
    glutDisplayFunc( __displayFunc );
    glutReshapeFunc( __reshapeFunc );
    glutKeyboardFunc( __keyboardFunc );
    glutKeyboardUpFunc( __keyboardUpFunc );
    glutSpecialFunc( __specialFunc );
    glutSpecialUpFunc( __specialUpFunc );
    glutMouseFunc( __mouseFunc );
    glutMotionFunc( __motionFunc );
    glutPassiveMotionFunc( __passiveMotionFunc );
    glutEntryFunc( __entryFunc );
    glutVisibilityFunc( __visibilityFunc );
    glutTimerFunc( 1, __timerFunc, 1 );
    
    window->initialize();
    return window;
}

void WindowManager::deleteWindow( Window &window )
{
    glutDestroyWindow( window.getID() );
    delete ( &window );
}


GRAPHICSLAB_NAMESPACE_END



