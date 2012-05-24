#ifndef GRAPHICSLAB_SCENE_HPP
#define GRAPHICSLAB_SCENE_HPP

//somehow I could not remove them
#include "../common/GLIncludes.hpp"
#include "../common/Types.hpp"
#include "../common/Time.hpp"
#include "../common/SoundEngine.hpp"

//std includes
#include <vector>

namespace game_space {

// Forward declarations
class Window;

//common forward declarations
class Camera2D;
class Camera3D;

/*
 Could not only use them. Don't know why.
class Time;
class RenderingParameters;
class SoundEngine;
*/

//illumination forward declarations
class Light;
class DirectionalLight;

//scene forward declarations
class Tank;
class SkyDome;
class Terrain;
class Water;

//projectile forward declarations
class Bullet;

//collision detection forward declarations
class Target;
class Projectile;

//AI forward declarations
class MessageBus;

//pathfinding forward declarations
class Node;


//shading engine forward declarations
class ShadingEngine;

/** Class that contains contents of the screen to display */
class Scene
{
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

	/** Reset scene data */
	void reset();

	// event handlers
	void onPaint();
	void onResize( int width, int height );
	void onMouseEntry( int state );
	void onMouseClick( int button, int state, int x, int y );
	void onMouseMove( int x, int y );
	void onMousePassiveMove( int x, int y );
	void onVisible( int state );
	void onTimer( int value );
	void onIdle();

	//getters and setters
	Terrain& getTerrain() const {return *_terrain;}
	SoundEngine getSoundEngine();
	void setPlayerTank(Tank* tank);
	Tank* getPlayerTank();
	Camera3D* getTankCam();
	Camera3D* getCurrentlyActiveCamera();
	MessageBus* getMessageBus();
	Water* getWater();
	std::vector<Target*> getTargets();
	Window& getWindow();

private:
	/**
	 * Update data for 'seconds' seconds
	 *
	 * @param[in]	seconds		Seconds to update simulation for
	 */
	void update( float seconds );

	// Draw parts of the scene
	void drawWaterImage();
	void drawScene();
	void drawGrid();
	void drawOverlay();

	void handleKeyboardInput();
public:
	std::vector<Target*> _targets;
	std::vector<Projectile*> _projectiles;
private:

	Window &_window;

	SoundEngine _soundEngine;

	bool _gridOn;
	GLuint _gridDisplayList;
	bool _dashBoardOn;

	// Scene data
	RenderingParameters _renderingParameters;

	Time _updateTime;
	bool _firstUpdate;

	int _mouseX, _mouseY;// Last mouse position

	enum CameraMode
	{
		INSIDE_CAM,
		TANK_CAM,
		OVERVIEW_CAM
	}_cameraMode;

	struct FreeCameraParameters
	{
		float radius;
		float azimuth;
		float elevation;
		float moveX, moveY;

		FreeCameraParameters() {reset();}

		void reset()
		{
			radius = 40.0f;
			azimuth = 180.0f;
			elevation = 10.0f;
			moveX = moveY = 0.0f;
		}

		void applyToCamera( Camera3D &camera );

	}_freeCameraParameters;

	Camera2D *_overlayCam;
	Camera3D *_tankCam;
	Camera3D *_overviewCam;
	Camera3D* _currentlyActiveCamera;
	bool _shadowsActive;

	typedef std::vector<Light*> LightVector;
	LightVector _lights;

	SkyDome *_skyDome;
	Terrain *_terrain;
	Water *_water;
	DirectionalLight* _sunLight;

	Node* _endNode;

	Tank* _playerTank;

	ShadingEngine* _shadingEngine;

	MessageBus* _messageBus;

}; // class Scene

}

#endif // GRAPHICSLAB_SCENE_HPP
