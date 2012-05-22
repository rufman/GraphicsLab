/**
 * application.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_CAMERA_HPP
#define GRAPHICSLAB_CAMERA_HPP

#include "defs.hpp"
#include "gl_includes.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

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
     
    const Viewport &getViewport() const { return _viewport; }
    void setViewport( const Viewport &viewport ) { _viewport = viewport; }
    void applyViewport() const { glViewport( _viewport.x, _viewport.y, _viewport.width, _viewport.height ); }
    
    virtual void applyProjection() const = 0;
    virtual void applyModelview() const = 0;
    
protected:
    Scene &_scene;
    Viewport _viewport;
    
}; // class Camera

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_CAMERA_HPP


