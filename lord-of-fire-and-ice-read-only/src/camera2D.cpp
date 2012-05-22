/**
 * camera2D.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "camera2D.hpp"

// Includes

GRAPHICSLAB_NAMESPACE_BEGIN

Camera2D::Camera2D( Scene &scene ) : 
    Camera( scene )
{
}

Camera2D::~Camera2D()
{
}

void Camera2D::applyProjection() const
{    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, _viewport.width, 0, _viewport.height );
}

void Camera2D::applyModelview() const
{    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 );
}

GRAPHICSLAB_NAMESPACE_END


