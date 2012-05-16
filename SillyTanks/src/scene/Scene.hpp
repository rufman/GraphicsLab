/**
 * scene.hpp
 * See scene.cpp
 */

#ifndef GRAPHICSLAB_SCENE_HPP
#define GRAPHICSLAB_SCENE_HPP

// commons includes
#include "../common/Definitions.hpp"
#include "../common/GLIncludes.hpp"
#include "../common/Types.hpp"
#include "../common/Time.hpp"
#include "../common/TGATexture.hpp"

//illumination include
#include "illumination/DirectionalLight.hpp"

//particle engine includes
#include "particleEngine/ParticleEngine.hpp"
#include "particleEngine/Smoke.hpp"

//pathfinding includes
#include "pathfinding/Node.hpp"

#include <vector>

GAME_NAMESPACE_BEGIN

// Forward declarations
class Window;
class Camera2D;
class Camera3D;
class Tank;
class Light;
class SkyDome;
class Bullet;
class Terrain;

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
	const Terrain& getTerrain() const {return *_terrain;}

private:
	/**
	 * Update data for 'seconds' seconds
	 *
	 * @param[in]	seconds		Seconds to update simulation for
	 */
	void update( float seconds );

	// Draw parts of the scene
	void drawScene();
	void drawGrid();
	void drawOverlay();
	void fireBullet();
	void handleKeyboardInput();
	Window& getWindow();

private:
	Window &_window;

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

	typedef std::vector<Light*> LightVector;
	LightVector _lights;

	SkyDome *_skyDome;
	Terrain *_terrain;
	DirectionalLight* _sunLight;
	Tank *_tank;

	ParticleEngine<Smoke> *_testParticles;

	std::vector<Bullet*> _bullets;
	Node* _endNode;

}; // class Scene

GAME_NAMESPACE_END

#endif // GRAPHICSLAB_SCENE_HPP
