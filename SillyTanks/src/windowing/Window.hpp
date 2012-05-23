#ifndef GRAPHICSLAB_WINDOW_HPP
#define GRAPHICSLAB_WINDOW_HPP

// Global definitions include
#include "../common/Definitions.hpp"

// std includes
#include <string>

namespace game_space {

// Forward declarations
class Scene;

class Window
{
public:
    friend class WindowManager;
    
    // Parameters
    struct Parameters
    {
    	int argc; char **argv;
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
    
    // event handlers for window events
    void onPaint();
    void onResize( int width, int height );
    void onVisible( int state );
    void onTimer( int value );
    void onIdle();

    //mouse event handlers
    void onMouseEntry( int state );
    void onMouseClick( int button, int state, int x, int y );
    void onMouseMove( int x, int y );
    void onMousePassiveMove( int x, int y );

    //key event handlers
    void resetKeys();
    bool keyPressed(char key);
    bool keyHit(char key);
    bool specialKeyPressed(int key);
    bool specialKeyHit(int key);
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
    // Parameters
    Parameters _parameters;
    
    // Window ID
    int _windowID;
    
    // scene
    Scene *_scene;
    
    //key and special key maps
	bool _keyPressed[256];// boolean map of normal keys
	bool _keyHit[256];
	bool _oldKeyPressed[256];

	bool _specialKeyPressed[246];//boolean maps for special keys
	bool _oldSpecialKeyPressed[246];
	bool _specialKeyHit[246];

}; // class Window


}


#endif // GRAPHICSLAB_WINDOW_HPP
