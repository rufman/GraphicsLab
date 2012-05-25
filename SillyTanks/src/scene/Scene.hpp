#ifndef GRAPHICSLAB_SCENE_HPP
#define GRAPHICSLAB_SCENE_HPP

//somehow I could not remove them
#include "../common/GLIncludes.hpp"
#include "../common/Types.hpp"
#include "../common/Time.hpp"
#include "../common/SoundEngine.hpp"
#include "../scene/illumination/shading/ShadingEngine.hpp"

#include "Fog.hpp"

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
class ShadingEngine;
*/

//illumination forward declarations
class Light;
class DirectionalLight;

//scene forward declarations
class Tank;
class SkyDome;
class Terrain;
class Water;
class Fog;

//projectile forward declarations
class Bullet;

//collision detection forward declarations
class Target;
class Projectile;

//AI forward declarations
class MessageBus;

//pathfinding forward declarations
class Node;


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
	ShadingEngine getShadingEngine();
	void setPlayerTank(Tank* tank);
	Tank* getPlayerTank();
	Camera3D* getTankCam();
	Camera3D* getCurrentlyActiveCamera();
	MessageBus* getMessageBus();
	Water* getWater();
	std::vector<Target*> getTargets();
	std::vector<Projectile*> getProjectiles();
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
	void drawOverlay();

	void handleKeyboardInput();

private:

	Window &_window;

	// Scene data
	RenderingParameters _renderingParameters;

	Time _updateTime;
	bool _firstUpdate;

	int _mouseX, _mouseY;// Last mouse position

	enum CameraMode
	{
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

	//toggles
	bool _shadowsActive;
	bool _fogActive;
	bool _shaderActive;

	bool _shadersAlreadyCompiled;

	//lights
	typedef std::vector<Light*> LightVector;
	LightVector _lights;
	DirectionalLight* _sunLight;


	//scene components
	SkyDome *_skyDome;
	Terrain *_terrain;
	Water *_water;
	Fog* _fog;

	Node* _endNode;

	Tank* _playerTank;
	std::vector<Target*> _targets;
	std::vector<Projectile*> _projectiles;

	ShadingEngine _shadingEngine;

	MessageBus* _messageBus;

	SoundEngine _soundEngine;

}; // class Scene

}

#endif // GRAPHICSLAB_SCENE_HPP
