/**
 * spotLight.cpp
* This class is creating a spotlight.
 */

// Class declaration include
#include "SpotLight.hpp"

// Includes
#include "../../common/Utils.hpp"
#include "../../common/Material.hpp"
#include "../../common/Definitions.hpp"

#include <cmath>


namespace game_space {

SpotLight::SpotLight( Scene &scene ) : 
	Light( scene ), 
	_spotDirection( Vector3D( 0, 0, -1 ) ), 
	_spotExponent( 0 ),
	_spotCutoff( 180 )
{     
}

SpotLight::~SpotLight()
{
}

void SpotLight::apply()
{
    Light::apply();
	
	glLightfv( _id, GL_SPOT_DIRECTION, _spotDirection() );
    glLightf( _id, GL_SPOT_EXPONENT, _spotExponent );
    glLightf( _id, GL_SPOT_CUTOFF, _spotCutoff );
}

void SpotLight::setSpotDirection( const Vector3D &spotDirection )
{ 
    _spotDirection = spotDirection; 
    Utils::normalize( _spotDirection );
}

void SpotLight::lookAt( const Point &target )
{
    Vector3D direction( target.x - _position.x, target.y - _position.y, target.z - _position.z );
    setSpotDirection( direction );
}

void SpotLight::setSpotExponent( const float spotExponent )
{ 
    _spotExponent = spotExponent;
}

void SpotLight::setSpotCutoff( const short spotCutoff )
{ 
    _spotCutoff = spotCutoff;
}

void SpotLight::draw() const
{
	if ( !isOn() )
		return;

	glShadeModel( ( _renderingParameters.shadeMode == RenderingParameters::FLAT ) ? GL_FLAT : GL_SMOOTH );
	glPolygonMode( GL_FRONT_AND_BACK, ( _renderingParameters.drawMode == RenderingParameters::WIREFRAME ) ? GL_LINE : GL_FILL );
	
	Material material;
	material.setEmission( _diffuse );
	material.setActive();
	
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glTranslatef( _position.x, _position.y, _position.z );
	Utils::applyGLRotation( Vector3D( 0.0, 0.0, -1.0 ), _spotDirection );
	glutSolidCone( 0.1*std::tan( Utils::toRadian( _spotCutoff ) ), 0.1, 10, 10 );
	glPopMatrix();
}


}
