/**
 * window.hpp
* See window.cpp
 */

#ifndef GRAPHICSLAB_WINDOW_HPP
#define GRAPHICSLAB_WINDOW_HPP


// Global definitions include
#include "../common/Definitions.hpp"

// Includes
#include <string>


GAME_NAMESPACE_BEGIN


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
            title( std::string( WINDOW_TITLE ) )
        {
        }
    };
    
    // EVENT HANDLERS
    void onPaint();
    void onResize( int width, int height );
    void onMouseEntry( int state );
    void onMouseClick( int button, int state, int x, int y );
    void onMouseMove( int x, int y );
    void onMousePassiveMove( int x, int y );
    void onVisible( int state );
    void onTimer( int value );
    void onIdle();
    void setKey(unsigned char key,bool value);
    void setSpecialKey(int key,bool value);
    
private:
    /** 
	 * Constructor
	 *
	 * @param[in]	parameters	Window parameters
	 *
	 */
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
    
	bool _keyPressed[256];// boolean map of normal keys
	bool _specialKeyStates[246]; // boolean map of special keys

}; // class Window


GAME_NAMESPACE_END


#endif // GRAPHICSLAB_WINDOW_HPP
