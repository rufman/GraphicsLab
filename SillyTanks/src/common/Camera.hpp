/**
 * camera.hpp
 This is an base class for 2D and 3D cameras. It defines what a camera must have independent on its form.
 */

#ifndef GAME_CAMERA_HPP
#define GAME_CAMERA_HPP

// Global definitions include
#include "../common/Definitions.hpp"

// Includes
#include "../common/GLIncludes.hpp"

namespace game_space {

// Forward declarations
class Scene;

/** Structure for a viewport */
struct Viewport
{
	int x, y, width, height;

	Viewport( int x_ = 0, int y_ = 0, int width_ = 0, int height_ = 0 ) :
	x( x_ ), y( y_ ), width( width_ ), height( height_ )
	{
	}
};

/** Base class to represent a camera */
class Camera
{
public:
	/**
	 * Constructor
	 *
	 * @param[in]   scene   Owning scene
	 */
	Camera( Scene &scene ) : _scene( scene ) {}

	/** Destructor */
	virtual ~Camera() {}

	const Viewport &getViewport() const {return _viewport;}
	void setViewport( const Viewport &viewport ) {_viewport = viewport;}
	void applyViewport() const {glViewport( _viewport.x, _viewport.y, _viewport.width, _viewport.height );}

	virtual void applyProjection() const = 0;
	virtual void applyModelview() const = 0;

protected:
	Scene &_scene;
	Viewport _viewport;

};

}

#endif // GAME_CAMERA_HPP
