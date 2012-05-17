/**
 * positionalLight.cpp
* This class creates a positional light.
 */

// Class declaration include
#include "PositionalLight.hpp"

// Includes
#include "../../common/Material.hpp"


namespace game_space {


PositionalLight::PositionalLight( Scene &scene ) : 
    Light( scene )
{
}

PositionalLight::~PositionalLight()
{
}


void PositionalLight::draw() const
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
	glutSolidSphere( 0.1, 10, 10 );
	glPopMatrix();
}


}
