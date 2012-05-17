/**
 * application.cpp
 * This class parses the arguments, initializes glut and the window used for drawing and runs the glut main loop.
 */
// Class declaration include
#include "Application.hpp"

// Includes
#include "../common/Exception.hpp"
#include "../common/GLIncludes.hpp"
#include "../windowing/WindowManager.hpp"

#include <cstdlib>
#include <iostream>


namespace game_space {

Application *Application::_instance = NULL;

Application &Application::getInstance()
{
    if ( _instance == NULL )
    {
        _instance = new Application();
    }
    
    return ( *_instance );
}

Application::Application() :
    _mainWindow( NULL )
{
}

Application::~Application()
{
    WindowManager &windowManager = WindowManager::getInstance();
    windowManager.deleteWindow( *_mainWindow );
    _mainWindow = NULL;
}

Application::Parameters::Parameters() :
		//set the window title to GUI TITLE
    windowTitle(std::string( WINDOW_TITLE ))
{
}

void Application::Parameters::parse( int argc, char **argv )
{    
	// Parse arguments (they will be set later on by the menu)
    for ( int argID = 1; argID < argc; )
    {
        std::string arg = argv[argID++];
        if ( arg == "-s" || arg == "-sky" )
        {
            skyTextureFile = argv[argID++];
        }		
		else if ( arg == "-t" || arg == "-terrain" )
        {
            terrainFilePrefix = argv[argID++];
        }
        else
        {
            std::cerr << "WARNING: Unknown application parameter \"" << arg << "\"" << std::endl;
        }
    }
}

void Application::initialize( int argc, char **argv )
{
    WindowManager &windowManager = WindowManager::getInstance();
    if ( _mainWindow )
    {
        windowManager.deleteWindow( *_mainWindow );
        _mainWindow = NULL;
    }
    
    _parameters.parse( argc, argv );
    
    // Initialize GLUT
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	
    Window::Parameters windowParameters;
    windowParameters.title = _parameters.windowTitle;
    _mainWindow = windowManager.createWindow( windowParameters );
}

int Application::run()
{    
    glutMainLoop();
    return EXIT_SUCCESS;
}


}
