/**
 * camera2D.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_CAMERA2D_HPP
#define GRAPHICSLAB_CAMERA2D_HPP

// Base class include
#include "camera.hpp"

// Includes

GRAPHICSLAB_NAMESPACE_BEGIN

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

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_CAMERA2D_HPP


