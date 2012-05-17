/**
 * light.cpp
 *
 */

// Class declaration include
#include "Light.hpp"

// Includes
#include "../../common/Exception.hpp"


namespace game_space {


Light::Light( Scene &scene ) :
	Drawable( scene ),
    _id( getLightID() ),
    _on( false ),
    _ambient( Color( 0.0, 0.0, 0.0, 1.0 ) ),
    _position( Point( 0.0, 0.0, 1.0 ) ),
    _constantAttenuation( 1.0 ),
    _linearAttenuation( 0.0 ),
    _quadraticAttenuation( 0.0 )
{
    if ( _id == GL_LIGHT0 )
    {
        _diffuse = Color( 1.0, 1.0, 1.0, 1.0 );
        _specular = Color( 1.0, 1.0, 1.0, 1.0 );
    }
    else
    {
        _diffuse = Color( 0.0, 0.0, 0.0, 1.0 );
        _specular = Color( 0.0, 0.0, 0.0, 1.0 );
    }


}

Light::~Light()
{
    releaseLightID( _id );
}


void Light::turnOn( bool on )
{
    _on = on;
    if ( on == true )
        glEnable( _id );
    else
        glDisable( _id );
}

bool Light::isOn() const
{
    return ( _on );
}


void Light::apply()
{
    if ( _on )
        glEnable( _id );
    else
        glDisable( _id );

    // Position
	float positionArray[4] = { _position.x, _position.y, _position.z, _position.w };
    glLightfv( _id, GL_POSITION, positionArray );

    // Color
	float ambientArray[4] = { _ambient.r, _ambient.g, _ambient.b, _ambient.a };
    glLightfv( _id, GL_AMBIENT, ambientArray );
	float diffuseArray[4] = { _diffuse.r, _diffuse.g, _diffuse.b, _diffuse.a };
    glLightfv( _id, GL_DIFFUSE, diffuseArray );
	float specularArray[4] = { _specular.r, _specular.g, _specular.b, _specular.a };
    glLightfv( _id, GL_SPECULAR, specularArray );

    // Attenuation
    glLightf( _id, GL_CONSTANT_ATTENUATION, _constantAttenuation );
    glLightf( _id, GL_LINEAR_ATTENUATION, _linearAttenuation );
    glLightf( _id, GL_QUADRATIC_ATTENUATION, _quadraticAttenuation );

}


void Light::setPosition( const Point &position )
{
    _position = position; _position.w = 1.0;
	float positionArray[4] = { _position.x, _position.y, _position.z, _position.w };
    glLightfv( _id, GL_POSITION, positionArray );
}


void Light::setAmbient( const Color &ambient )
{
    _ambient = ambient;
	float ambientArray[4] = { _ambient.r, _ambient.g, _ambient.b, _ambient.a };
    glLightfv( _id, GL_AMBIENT, ambientArray );
}

void Light::setDiffuse( const Color &diffuse )
{
    _diffuse = diffuse;
	float diffuseArray[4] = { _diffuse.r, _diffuse.g, _diffuse.b, _diffuse.a };
    glLightfv( _id, GL_DIFFUSE, diffuseArray );
}

void Light::setSpecular( const Color &specular )
{
    _specular = specular;
	float specularArray[4] = { _specular.r, _specular.g, _specular.b, _specular.a };
    glLightfv( _id, GL_SPECULAR, specularArray );
}

void Light::setConstantAttenuation( const float constantAttenuation )
{
    _constantAttenuation = constantAttenuation;
    glLightf( _id, GL_CONSTANT_ATTENUATION, _constantAttenuation );
}

void Light::setLinearAttenuation( const float linearAttenuation )
{
    _linearAttenuation = linearAttenuation;
    glLightf( _id, GL_LINEAR_ATTENUATION, _linearAttenuation );
}

void Light::setQuadraticAttenuation( const float quadraticAttenuation )
{
    _quadraticAttenuation = quadraticAttenuation;
    glLightf( _id, GL_QUADRATIC_ATTENUATION, _quadraticAttenuation );
}


// Static functions
Light::LightAllocationMap &Light::initLightAllocationMap()
{
    static LightAllocationMap map;
    map[GL_LIGHT0] = map[GL_LIGHT1] = map[GL_LIGHT2] = map[GL_LIGHT3] = map[GL_LIGHT4] = map[GL_LIGHT5] = map[GL_LIGHT6] = map[GL_LIGHT7] = false;

    return map;
}
Light::LightAllocationMap &Light::lightAllocationMap = Light::initLightAllocationMap();



GLenum Light::getLightID()
{
    for ( LightAllocationMap::iterator lightIter = lightAllocationMap.begin();
         lightIter != lightAllocationMap.end();
         ++lightIter )
    {
        GLenum light = lightIter->first;
        bool lightAllocated = lightIter->second;
        if ( false == lightAllocated )
        {
            lightAllocationMap[light] = true;
            return light;
        }
    }

    throw Exception( "Not enough lights in OpenGL environment" );
}


void Light::releaseLightID( GLenum id )
{
    LightAllocationMap::iterator lightIter = lightAllocationMap.find( id );
    if ( lightIter == lightAllocationMap.end() )
        throw Exception( "Undefined light ID" );

    lightIter->second = false;
    glDisable( id );
}


}
