/**
 * directionalLight.cpp
 * This class creates a directional light.
 */

// Class declaration include
#include "DirectionalLight.hpp"

// Includes
#include "../../common/Utils.hpp"
#include "../../common/Material.hpp"


namespace game_space {


DirectionalLight::DirectionalLight( Scene &scene ) : Light( scene )
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::setPosition( const Point &position ) 
{ 
    _position = position; _position.w = 0.0;
    glLightfv( _id, GL_POSITION, _position() );
}

void DirectionalLight::setConstantAttenuation( const float constantAttenuation )
{ 
    _constantAttenuation = 1.0; 
    glLightf( _id, GL_CONSTANT_ATTENUATION, _constantAttenuation );
}

void DirectionalLight::setLinearAttenuation( const float linearAttenuation )
{ 
    _linearAttenuation = 0.0;            
    glLightf( _id, GL_LINEAR_ATTENUATION, _linearAttenuation );
}

void DirectionalLight::setQuadraticAttenuation( const float quadraticAttenuation )
{ 
    _quadraticAttenuation = 0.0;
    glLightf( _id, GL_QUADRATIC_ATTENUATION, _quadraticAttenuation );
}

void DirectionalLight::draw() const
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

	Vector3D normalized( -_position.x, -_position.y, -_position.z );
	Utils::normalize( normalized );

	glTranslatef( _position.x, _position.y, _position.z );
	Utils::applyGLRotation( Vector3D( 0.0, 0.0, -1.0 ), normalized );

	float length = 0.3;
	glBegin( GL_LINES );
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( 0.0, 0.0, -length );
	glEnd();
	glTranslatef( 0.0, 0.0, -length );
	glRotatef( 180, 0.0, 1.0, 0.0 );
	glutSolidCone( 0.05, 0.1, 20, 5 );
	
	glPopMatrix();

}


}
