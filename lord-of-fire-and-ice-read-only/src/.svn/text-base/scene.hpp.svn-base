/**
 * scene.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_SCENE_HPP
#define GRAPHICSLAB_SCENE_HPP

// Includes
#include "defs.hpp"
#include "types.hpp"
#include "time.hpp"
#include "sector.hpp"
#include "map.hpp"
#include "player.hpp"
#include "inputManager.hpp"
#include "button.hpp"
#include "menu.hpp"
#include "sky.hpp"
#include "fog.hpp"
#include "loadingScreen.hpp"
#include "menuManager.hpp"
#include "sun.hpp"
#include "dock.hpp"
#include "graphicsSettings.hpp"
#include "gameSaveFunctions.hpp"
#include "npcManager.hpp"




GRAPHICSLAB_NAMESPACE_BEGIN

// Forward declarations
class Window;
class Camera2D;
class Camera3D;
class Player;
class Map;
class Sky;
class Fog;
class IntroScreen;
class LoadingScreen;
class MenuManager;
class Sun;
class Dock;
class GraphicsSettings;
class GameSaveFunctions;
class NPCManager;


/** Class that contains contents of the screen to display */
class Scene {

public:
    /** 
     * Constructor. 
     *
     * @param[in]   window      Owning window
     */
    Scene( Window &window );
    
    /** Destructor */
    ~Scene();
    
    // Initialize scene data
    void initialize();
    void postIntroInitialize();
    void setClearColor();
    
    /** Reset scene data */
    void reset();
	
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

    static Color3f getBackgroundColor();
	static bool isMapDebugOn();

	void loadIntroMap();
	void loadGame(int gameSave);
	void saveGame(int gameSave);

	void cheat();

    
private:

    // Draw the game world
    void drawWorld();

    // Draw the shadow texture
    void drawShadowTexture();
    
    // Draw the reflection texture
    void drawReflectionTexture();

    // Draw the interface
    void drawInterface();
	
	//draw Dock
	void drawDock();


private:
	static const bool DEBUG_MAP = false;

	static Color3f _backgroundColor;

    Window &_window;
    
    Time _updateTimer;

    InputManager* _inputManager;

    MenuManager* _menuManager;
	
	Dock* _dock;

    Map* _map;
    Sky* _sky;
    Player* _player;
    Fog* _fog;

    Camera2D *_camera2D;
    Camera3D *_camera3D;

    IntroScreen* _introScreen;

    LoadingScreen* _loadingScreen;

    Sun* _sun;
    
    GraphicsSettings* _graphicsSettings;

    GameSaveFunctions* _gameSaveFunctions;

    NPCManager* _npcManager;

}; // class Scene

GRAPHICSLAB_NAMESPACE_END

#endif  //  GRAPHICSLAB_SCENE_HPP


