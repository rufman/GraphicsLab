/**
 * main.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Includes
#include "exception.hpp"
#include "application.hpp"
#include <iostream>
GRAPHICSLAB_NAMESPACE_USE

//////////////////////////////////////////////////////////////////////////////
// Signal handler
//////////////////////////////////////////////////////////////////////////////
#include <csignal>
#include <cstdlib>
void signalHandler(int signal)
{
    std::cout << "Caught signal " << signal << "... Exiting..." << std::endl;
    exit( EXIT_SUCCESS );
}
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Main function
//////////////////////////////////////////////////////////////////////////////

int main( int argc, char *argv[] )
{
    // Signal handler
	signal(SIGINT, signalHandler);
    try
    {
        Application &application = Application::getInstance();        
        application.initialize( argc, argv );
        
        return application.run();
    }
    catch ( const Exception &e )
    {
        std::cerr << "Exception : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch ( ... )
    {
        std::cerr << "Exception : Unknown exception" << std::endl;
        return EXIT_FAILURE;
    }

    
    return EXIT_SUCCESS;
};

