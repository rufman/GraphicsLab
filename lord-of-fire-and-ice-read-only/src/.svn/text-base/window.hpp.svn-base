/**
 * window.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_WINDOW_HPP
#define GRAPHICSLAB_WINDOW_HPP

// Includes
#include "defs.hpp"

#include <string>

GRAPHICSLAB_NAMESPACE_BEGIN

// Forward declarations
class Scene;

/** Class for a window */
class Window
{
public:
    friend class WindowManager;
    
    /** Parameters */
    struct Parameters
    {
        int width, height;
        int posX, posY;
        bool fullScreen;
        std::string title;
        
        Parameters() :
            width( 1024 ), height( 768 ),
            posX( -1 ), posY( -1 ),
            fullScreen( false ),
            title( std::string( "Lord of Fire & Ice" ) )
        {
        }
    };

    // EVENT HANDLERS
    void onPaint();
    void onResize( int width, int height );
    void onKey( unsigned char key );
    void onKeyUp( unsigned char key );
    void onSpecialKey( int key );
    void onSpecialKeyUp( int key );
    void onMouseEntry( int state );
    void onMouseClick( int button, int state, int x, int y );
    void onMouseMove( int x, int y );
    void onMousePassiveMove( int x, int y );
    void onVisible( int state );
    void onTimer( int value );
    void onIdle();
    
    void fullscreenOn();
    void fullscreenOff();

private:
    /** Constructor */
    Window( const Parameters &parameters );
    
    /** Destructor */
    ~Window();

    /** Initialize window */
    void initialize();
    
    /**
     * Get window ID.
     *
     * @return      Window ID
     */
    int getID() const { return _windowID; }
    
    /** 
     * Set window ID.
     *
     * @param[in]   windowID    Window ID
     */
    void setID( int windowID );

private:
    /** Parameters */
    Parameters _parameters;

    /** Window ID */
    int _windowID;
    
    /** Scene data */
    Scene *_scene;
        
}; // class Window

GRAPHICSLAB_NAMESPACE_END

#endif  //  GRAPHICSLAB_WINDOW_HPP


