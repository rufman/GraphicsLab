/**
 * bullet.cpp
 * This class creates bullets that have a position and a velocity that can be set when shot. They fly according a parabola like an actual object.
 */

// Class declaration include
#include "Bullet.hpp"

// common includes
#include "../../../common/GLIncludes.hpp"

//scene includes
#include "../../Scene.hpp"
#include "../../Terrain.hpp"

namespace game_space {

Bullet::Bullet( Scene &scene, float size ) :
Projectile( scene,Projectile::BULLET,0 ),
_size( size ) {}

Bullet::~Bullet() {}

void Bullet::draw() const
{
	glShadeModel( _renderingParameters.shadeMode == RenderingParameters::FLAT ? GL_FLAT : GL_SMOOTH );
	glPolygonMode( GL_FRONT_AND_BACK, _renderingParameters.drawMode == RenderingParameters::WIREFRAME ? GL_LINE : GL_FILL );

	glMatrixMode( GL_MODELVIEW );

	glPushMatrix();
	glTranslatef( _position.x, _position.y, _position.z );

	/*float materialAmbient[3] = {0.4, 0.1, 0.1};
	float materialDiffuse[3] = {0.6, 0.2, 0.2};
	float materialSpecular[3] = {0.8, 0.4, 0.4};
	float materialEmission[3] = {0.2, 0.0, 0.0};
	int shininess = 50;

	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, materialEmission );
	glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, shininess );*/

	glutSolidCube( _size );

	glPopMatrix();
}

void Bullet::setVelocity( const Vector3D &velocity )
{
	_velocity = velocity;
}

void Bullet::move( float seconds )
{
	//ballistics of a projectile
	_position.x += _velocity.x*seconds;
	_position.y += _velocity.y*seconds + 0.5*GRAVITATIONAL_ACCELERATION*seconds*seconds;
	_position.z += _velocity.z*seconds;

	_velocity.y += GRAVITATIONAL_ACCELERATION*seconds;

	//if the bullet hits the ground it detonates
	if ((_position.y < _scene.getTerrain().getHeight(_position))) {
		_scene.getTerrain().doDamageAt(_position, 0.05);
		_detonated = true;
	}
}


bool Bullet::isDetonated() {
	return _detonated;
}

}
