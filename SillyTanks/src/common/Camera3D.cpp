/**
 * camera3D.cpp
 * This class creates a 3D camera
 */

// Class declaration include
#include "Camera3D.hpp"

namespace game_space {

Camera3D::Camera3D( Scene &scene ) :
Camera( scene ){}

Camera3D::~Camera3D(){}

void Camera3D::applyProjection() const
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 90.0, _viewport.width*1.0/_viewport.height, 0.1, 2000.0 );
}

void Camera3D::applyModelview() const
{
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( _lookAt.from.x, _lookAt.from.y, _lookAt.from.z,
			_lookAt.to.x, _lookAt.to.y, _lookAt.to.z,
			_lookAt.up.x, _lookAt.up.y, _lookAt.up.z );
}

}
