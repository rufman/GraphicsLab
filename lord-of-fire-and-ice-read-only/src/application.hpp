/**
 * application.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_APPLICATION_HPP
#define GRAPHICSLAB_APPLICATION_HPP

// Includes
#include "defs.hpp"

#include <string>

GRAPHICSLAB_NAMESPACE_BEGIN

// Forward declarations
class Window;

/** Class for an application */
class Application
{
public:

    
    
    /**
     * Get the singleton instance.
     *
     * @return      Singleton instance reference
     */
    static Application &getInstance();

    
    /**
     * Initialize application.
     *
     * @param[in]   argc    Command line argument count
     * @param[in]   argv    Command line arguments
     */
    void initialize( int argc, char **argv );
    
    /**
     * Run application main loop.
     *
     * @return      Return status value
     */
    int run();
    
    
private:
    /** Constructor */
    Application();
    
    /** Destructor */
    ~Application();    
    
    
private:
    /** Singleton instance */
    static Application *_instance;
    
    /** Main window */
    Window *_mainWindow;    
    
}; // class Application

GRAPHICSLAB_NAMESPACE_END

#endif  //  GRAPHICSLAB_APPLICATION_HPP


