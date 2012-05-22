/**
 * directionalLight.cpp
 *
 * Author   :   Fatih Erol
 * Date     :   04.04.2011
 *
 * All rights reserved.
 */


// Class declaration include
#include "directionalLight.hpp"

// Includes
#include "utils.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

DirectionalLight::DirectionalLight( Scene &scene ) : Light( scene )
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::setPosition( const Point &position ) 
{ 
    _position = position; _position.w = 0.0;
	float positionArray[4] = { _position.x, _position.y, _position.z, _position.w };
    glLightfv( _id, GL_POSITION, positionArray );
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
}


GRAPHICSLAB_NAMESPACE_END


