/**
 * camera3D.hpp
* See camera3D.cpp
 */

#ifndef GRAPHICSLAB_CAMERA3D_HPP
#define GRAPHICSLAB_CAMERA3D_HPP


// Base class include
#include "Camera.hpp"

// Includes
#include "../common/Types.hpp"


namespace game_space {


struct LookAt
{
	Point from;
	Point to;
	Vector3D up;
	
	LookAt( const Point &from_ = Point( 0.0, 0.0, 0.0 ), const Point &to_ = Point( 0.0, 0.0, -1.0 ), const Vector3D &up_ = Vector3D( 0.0, 1.0, 0.0 ) ) :
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
    
    void applyProjection() const;
    void applyModelview() const;
	
private:
	LookAt _lookAt;

}; // class Camera3D


}


#endif // GRAPHICSLAB_CAMERA3D_HPP
