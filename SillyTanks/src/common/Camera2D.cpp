/**
 * camera2D.cpp
 * This class creates a 2D camera.
 */

// Class declaration include
#include "Camera2D.hpp"

namespace game_space {

Camera2D::Camera2D( Scene &scene ) :
Camera( scene ) {}

Camera2D::~Camera2D() {}

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

}
