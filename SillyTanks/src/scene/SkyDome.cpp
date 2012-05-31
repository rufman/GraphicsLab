/**
 * skyDome.cpp
* This class creates a sky dome that can rotate the sky around you. Simulates the sky.
 */

// Class declaration include
#include "SkyDome.hpp"

// common includes
#include "../common/GLIncludes.hpp"
#include "../common/Utils.hpp"
#include "../common/Material.hpp"
#include "../common/TGATexture.hpp"

#include <cmath>

namespace game_space {

SkyDome::SkyDome( Scene &scene, const std::string &textureFile, float radius, uint numLongitudes, uint numLatitudes ) :
Drawable( scene ),
_radius( radius ),
_numLongitudes( numLongitudes ),
_numLatitudes( numLatitudes ),
_texture( new TGATexture( textureFile ) ),
_textureDisplacement( 0.0f )
{
	// define theta and phi
	//theta down-top angle
	float theta = Utils::PI / 2.0f / ((float)_numLongitudes-1);

	//phi left-right angle
	float phi = Utils::PI * 2.0f / ((float)_numLongitudes-1);

	//create vertices for the hemisphere
	for (uint i = 0; i < _numLatitudes; i++ ) {
		for (uint j = 0; j < _numLongitudes; j++ ) {
			_vertices.push_back( Point( _radius * cos( theta * i ) * cos(phi * j ),
							_radius * sin(theta * i ),
							_radius * cos(theta * i) * sin( phi * j ) ) );
		}
	}

	// add quads and calculate their normals
	for (uint j = 0; j < (_numLongitudes ) * (_numLatitudes - 1); j++ ) {
		//add quads
		if(j < (_numLongitudes ) * (_numLatitudes - 2))
		{
			//normal quads
			_quads.push_back(Quad( j, j + 1, j + _numLongitudes, j + _numLongitudes + 1));
		}
		else
		{
			//pole
			_quads.push_back( Quad( j, j + 1, j + _numLongitudes, j + _numLongitudes));
		}

		//calculate normal
		Vector3D phiVector = Vector3D( _vertices[j + 1].x - _vertices[j].x, _vertices[j + 1].y - _vertices[j].y, _vertices[j + 1].z - _vertices[j].z );
		Vector3D thetaVector = Vector3D( _vertices[j + _numLongitudes].x - _vertices[j].x, _vertices[j + _numLongitudes].y - _vertices[j].y, _vertices[j + _numLongitudes].z - _vertices[j].z );
		Vector3D quadNormal = Utils::cross( phiVector, thetaVector );

		Utils::normalize( quadNormal );
		_quadNormals.push_back( quadNormal );
	}

	// calculate vertex normals
	for (uint i = 0; i < _vertices.size() - 1; i++ ) {
		Vector3D vertexNormal;
		if ( i < _numLongitudes ) {
			if ( i == 0 ) {
				Vector3D quadNormal1 = _quadNormals[ i ];
				Vector3D quadNormal2 = _quadNormals[ i + _numLongitudes - 1 ];
				vertexNormal = Vector3D( quadNormal1.x + quadNormal2.x, quadNormal1.y + quadNormal2.y, quadNormal1.z + quadNormal2.z );
			}
			else {
				Vector3D quadNormal1 = _quadNormals[ i-1 ];
				Vector3D quadNormal2 = _quadNormals[ i ];
				vertexNormal = Vector3D( quadNormal1.x + quadNormal2.x, quadNormal1.y + quadNormal2.y, quadNormal1.z + quadNormal2.z );
			}
		}
		else if( i > _vertices.size() - _numLongitudes - 1) {
			if (i == _vertices.size() -1 ) {
				vertexNormal = _quadNormals[ i - _numLongitudes -1];
			}
			else {
				vertexNormal = _quadNormals[ i - _numLongitudes ];
			}
		}
		else {
			if ( i == _numLongitudes ) {
				Vector3D quadn1 = _quadNormals[ i ];
				Vector3D quadn2 = _quadNormals[ i - _numLongitudes ];
				Vector3D quadn3 = _quadNormals[ i - 1 ];
				Vector3D quadn4 = _quadNormals[ i + _numLongitudes -1 ];
				vertexNormal = Vector3D( quadn1.x + quadn2.x + quadn3.x + quadn4.x, quadn1.y + quadn2.y + quadn3.y + quadn4.y, quadn1.z + quadn2.z + quadn3.z + quadn4.z );
			}
			else {
				Vector3D quadn1 = _quadNormals[ i ];
				Vector3D quadn2 = _quadNormals[ i - _numLongitudes ];
				Vector3D quadn3 = _quadNormals[ i - 1 ];
				Vector3D quadn4 = _quadNormals[ i - _numLongitudes -1 ];
				vertexNormal = Vector3D( quadn1.x + quadn2.x + quadn3.x + quadn4.x, quadn1.y + quadn2.y + quadn3.y + quadn4.y, quadn1.z + quadn2.z + quadn3.z + quadn4.z );
			}
		}
		Utils::normalize( vertexNormal );
		_vertexNormals.push_back( vertexNormal );
	}
}

SkyDome::~SkyDome()
{
	delete _texture;
}

void SkyDome::reset()
{
	_textureDisplacement = 0.0f;
}

void SkyDome::update( float seconds )
{
	_textureDisplacement += seconds;
}

Point SkyDome::getSunPosition() const
{
	float heightAngle = Utils::PI/3.1f;
	float sunPosition = Utils::PI/0.955;
	float sun_radius = _radius*15/16;

	return Point( sun_radius * cos( heightAngle) * cos(-_textureDisplacement/64.0f+sunPosition ),
			sun_radius * sin(heightAngle),
			sun_radius * cos(heightAngle) * sin(-_textureDisplacement/64.0f+sunPosition ) );
}

float SkyDome::getDisplacement() const
{
	return _textureDisplacement;
}

void SkyDome::draw() const
{
	glShadeModel( ( _renderingParameters.shadeMode == RenderingParameters::FLAT ) ? GL_FLAT : GL_SMOOTH );
	glPolygonMode( GL_FRONT_AND_BACK, ( _renderingParameters.drawMode == RenderingParameters::WIREFRAME ) ? GL_LINE : GL_FILL );

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	_texture->setActive( true );

	glRotatef( _textureDisplacement, 0.0, 1.0, 0.0 );
	glBegin( GL_QUADS );

	float x = _texture->getWidth()/(_numLongitudes);
	float y = _texture->getHeight()/(_numLatitudes);
	for (uint i = 0; i < _quads.size(); i++ ) {
		glTexCoord2f( x * (i % _numLongitudes), _texture->getHeight() - y * ((int) i/_numLatitudes) );
		glVertex3f( _vertices[_quads[i].vertex1].x, _vertices[_quads[i].vertex1].y, _vertices[_quads[i].vertex1].z );

		glTexCoord2f( x * (i % _numLongitudes) + x, _texture->getHeight() - y * ((int) i/_numLatitudes) );
		glVertex3f( _vertices[_quads[i].vertex2].x, _vertices[_quads[i].vertex2].y, _vertices[_quads[i].vertex2].z );

		glTexCoord2f( x * (i % _numLongitudes) + x, _texture->getHeight() - y * ((int) i/_numLatitudes) - y);
		glVertex3f( _vertices[_quads[i].vertex4].x, _vertices[_quads[i].vertex4].y, _vertices[_quads[i].vertex4].z );

		glTexCoord2f( x * (i % _numLongitudes), _texture->getHeight() - y * ((int) i/_numLatitudes) - y);
		glVertex3f( _vertices[_quads[i].vertex3].x, _vertices[_quads[i].vertex3].y, _vertices[_quads[i].vertex3].z );
	}
	glEnd();
	// Disable texture
	_texture->setActive( false );

	glPopMatrix();
}

}
