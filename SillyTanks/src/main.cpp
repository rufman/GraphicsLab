/**
 * main.cpp
* This class is the main class that runs an instance of the application and provides a signal handler for all the exceptions that and one else handles.
 */

// Includes
#include "common/Exception.hpp"
#include "windowing/Application.hpp"

#include <iostream>

//##########
// Signal handler
//##########
#include <csignal>
#include <cstdlib>

GAME_NAMESPACE_USE

void signalHandler(int signal)
{
	//The signal handler quits if it receives a signal
    std::cout << "### Caught signal " << signal << "... Exiting... ###" << std::endl;
    exit( EXIT_SUCCESS );
}


//#########
// Main function
//#########
int main( int argc, char *argv[] )
{
    // set the signal handler to the one provided by this class
	signal(SIGINT, signalHandler);

	//for all the random calls inside of the project, we set the seed to time(0)
	srand((unsigned) time(0));
    
    try
    {
    	//create an instance of this application
        Application &application = Application::getInstance();        

        //initialize the application with the parameters we received from commandline
        application.initialize( argc, argv );
        
        //return if the application exited successfully
        return application.run();
    }
    catch ( const Exception &e )
    {
    	//caught a known exception of type exception
        std::cerr << "Exception : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch ( ... )
    {
    	//caught an unknown type of exception
        std::cerr << "Exception : Unknown exception" << std::endl;
        return EXIT_FAILURE;
    }
    
    //if the application did not exit with failure until here, it must have exited with success.
    return EXIT_SUCCESS;
}
