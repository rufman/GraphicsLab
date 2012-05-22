/**
 * windowManager.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_WINDOWMANAGER_HPP
#define GRAPHICSLAB_WINDOWMANAGER_HPP

// Includes
#include "defs.hpp"
#include "window.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

/** Class handles windows through GLUT */
class WindowManager
{
public:
    /**
     * Get the singleton instance.
     *
     * @return      Singleton instance reference
     */
    static WindowManager &getInstance();
    
    
    /**
     * Create a new window.
     *
     * @param[in, out]  parameters      Window parameters
     *
     * @return          Pointer to created window
     */
    Window *createWindow( Window::Parameters &parameters );
    
    /**
     * Delete a window.
     *
     * @param[in, out]  window      Window to destroy
     */
    void deleteWindow( Window &window );

    /**
     * Get the currently active window.
     *
     * @return      Currently active window.
     */
    Window *getActiveWindow() const { return _activeWindow; }
    
private:
    /** Constructor */
    WindowManager();
    
    /** Destructor */
    ~WindowManager();
    
private:
    /** Singleton instance */
    static WindowManager *_instance;
    
private:
    /** Currently active window */
    Window *_activeWindow;
    
}; // class WindowManager

GRAPHICSLAB_NAMESPACE_END

#endif  //  GRAPHICSLAB_WINDOWMANAGER_HPP


