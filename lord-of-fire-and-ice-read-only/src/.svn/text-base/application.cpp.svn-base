/**
 * application.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "application.hpp"

// Includes
#include "gl_includes.hpp"
#include "windowManager.hpp"

#include <cstdlib>
#include <iostream>

GRAPHICSLAB_NAMESPACE_BEGIN	


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
}

void Application::initialize( int argc, char **argv )
{
    
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    
    WindowManager &windowManager = WindowManager::getInstance();
    Window::Parameters windowParameters;
    
    _mainWindow = windowManager.createWindow( windowParameters );
}

int Application::run()
{    
    glutMainLoop();
    return EXIT_SUCCESS;
}


GRAPHICSLAB_NAMESPACE_END



