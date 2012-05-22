/**
 * camera3D.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "camera3D.hpp"

// Includes

GRAPHICSLAB_NAMESPACE_BEGIN

Camera3D::Camera3D( Scene &scene ) : 
    Camera( scene ),
    _fieldOfView(90.0)
{
}

Camera3D::~Camera3D()
{
}

void Camera3D::applyProjection() const
{ 
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( _fieldOfView, _viewport.width*1.0/_viewport.height, 0.3, 15000.0 );
}

void Camera3D::applyModelview() const
{    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	gluLookAt( _lookAt.from.x, _lookAt.from.y, _lookAt.from.z,
				_lookAt.to.x, _lookAt.to.y, _lookAt.to.z,
				_lookAt.up.x(), _lookAt.up.y(), _lookAt.up.z() );
}

void Camera3D::setFieldOfView(float fieldOfView) {
	_fieldOfView = fieldOfView;
}

GRAPHICSLAB_NAMESPACE_END


