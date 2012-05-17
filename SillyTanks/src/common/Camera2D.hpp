/**
 * camera2D.hpp
* See camera2D.cpp
 */

#ifndef GRAPHICSLAB_CAMERA2D_HPP
#define GRAPHICSLAB_CAMERA2D_HPP

// Base class include
#include "Camera.hpp"


namespace game_space {


/** Class to represent a 2D orthogonal camera */
class Camera2D : public Camera
{
public:
    /** 
     * Constructor 
     *
     * @param[in]   scene   Owning scene
     */
    Camera2D( Scene &scene );
    
    /** Destructor */
    ~Camera2D();
    
    
    void applyProjection() const;
    void applyModelview() const;
    
}; // class Camera2D


}


#endif // GRAPHICSLAB_CAMERA2D_HPP
