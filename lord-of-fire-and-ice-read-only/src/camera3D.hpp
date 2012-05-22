/**
 * camera3D.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_CAMERA3D_HPP
#define GRAPHICSLAB_CAMERA3D_HPP

// Base class include
#include "camera.hpp"

// Includes
#include "types.hpp"
#include "vector.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

struct LookAt
{
	Point from;
	Point to;
	vec3f up;

	LookAt( const Point &from_ = Point( 0.0, 0.0, 0.0 ), const Point &to_ = Point( 0.0, 0.0, -1.0 ), const vec3f &up_ = vec3f( 0.0, 1.0, 0.0 ) ) :
		from( from_ ), to( to_ ), up( up_ )
	{
	}
};

/** Class to represent a 3D perspective camera */
class Camera3D : public Camera
{
public:
    /** 
     * Constructor 
     *
     * @param[in]   scene   Owning scene
     */
    Camera3D( Scene &scene );
    
    /** Destructor */
    ~Camera3D();
    

	const LookAt &getLookAt() const { return _lookAt; }
	void setLookAt( const LookAt &lookAt ) { _lookAt = lookAt; }

	void setFieldOfView(float fieldOfView);
    
    void applyProjection() const;
    void applyModelview() const;

private:
	LookAt _lookAt;
	float _fieldOfView;
    
}; // class Camera3D

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_CAMERA3D_HPP


