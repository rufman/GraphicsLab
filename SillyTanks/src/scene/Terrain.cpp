/**
 * terrain.cpp
* This class creates a terrain that is read from a heightmap.
 */

// Class declaration include
#include "Terrain.hpp"

// common includes
#include "../common/GLIncludes.hpp"
#include "../common/Utils.hpp"
#include "../common/TGAImage.hpp"
#include "../common/TGATexture.hpp"

#include <sstream>
#include <cmath>
#include <iostream>

GAME_NAMESPACE_BEGIN

Terrain::Terrain( Scene &scene, const std::string &textureFilePrefix, float width, float length, uint widthResolution, uint lengthResolution ) :
Drawable( scene ),
_numDisplayLists( 2 ),
_width( width), _length( length ),
_widthResolution( widthResolution ), _lengthResolution( lengthResolution )
{
	_displayLists = glGenLists( _numDisplayLists );

	_material.setAmbient( Color( 0.2, 0.2, 0.2 ) );
	_material.setDiffuse( Color( 0.4, 0.4, 0.4 ) );
	_material.setSpecular( Color( 0.6, 0.6, 0.6 ) );
	_material.setEmission( Color( 0.0, 0.0, 0.0 ) );
	_material.setShininess( 32 );

	std::stringstream heightFieldFileName; heightFieldFileName << textureFilePrefix << "_hf.tga";
	std::stringstream textureFileName; textureFileName << textureFilePrefix << "_tex.tga";

	_heightData = new TGAImage();
	_heightData->load( heightFieldFileName.str() );
	_texture = new TGATexture( textureFileName.str() );

	if ( _widthResolution == 0 ) _widthResolution = _heightData->getWidth();
	if ( _lengthResolution == 0 ) _lengthResolution = _heightData->getHeight();

	_numVertices = _widthResolution*_lengthResolution;
	_vertexNormals = new Vector3D[_numVertices];

	_numTriangles = 2*( _widthResolution - 1 )*( _lengthResolution - 1 );
	_triangleNormals = new Vector3D[_numTriangles];

	float tex_x = _texture->getWidth()/_widthResolution;
	float tex_z = _texture->getHeight()/_lengthResolution;

	// Calculate vertices
	for (uint widthPoint = 0; widthPoint < _widthResolution; widthPoint++ )
	{
		for ( uint lengthPoint = 0; lengthPoint < _lengthResolution; lengthPoint++ )
		{
			uchar r = _heightData->getData()[ (lengthPoint + widthPoint*_lengthResolution)*4];
			uchar g = _heightData->getData()[ (lengthPoint + widthPoint*_lengthResolution)*4 + 1];
			uchar b = _heightData->getData()[ (lengthPoint + widthPoint*_lengthResolution)*4 + 2];
			float greyscale = (0.30 * (float)r + 0.59 * (float)g + 0.11 * (float)b)/40;

			_vertices.push_back( Point( -_width/2.0 + widthPoint*( _width/ _widthResolution  ), greyscale, -_length/2.0 + lengthPoint*( _length/_lengthResolution ) ) );
			_textureCoordinates.push_back( Point( tex_x * lengthPoint, tex_z * widthPoint ));

		}
	}

	// Build triangles
	for ( int widthPoint = 0; widthPoint < ( _widthResolution - 1 ); widthPoint++ )
	{
		for ( int lengthPoint = 0; lengthPoint < ( _lengthResolution - 1 ); lengthPoint++ )
		{
			int v1 = ( lengthPoint )*_widthResolution + ( widthPoint );
			int v2 = v1 + 1;
			int v3 = v2 + _widthResolution;
			int v4 = v3 - 1;
			_triangles.push_back(Triangle( v1, v2, v3 ));
			_triangles.push_back(Triangle(v1,v3,v4));
		}
	}

	// Calculate triangle normals
	for ( int widthPoint = 0; widthPoint < ( _widthResolution - 1 ); widthPoint++ )
	{
		for ( int lengthPoint = 0; lengthPoint < ( _lengthResolution - 1 ); lengthPoint++ )
		{
			int t1 = 2*lengthPoint*( _widthResolution - 1 ) + widthPoint*2;
			int t2 = t1 + 1;
			Triangle &triangle1 = _triangles[t1];
			Triangle &triangle2 = _triangles[t2];

			_triangleNormals[t1] =Utils::normal( _vertices[triangle1.vertex1], _vertices[triangle1.vertex2], _vertices[triangle1.vertex3] );

			_triangleNormals[t2] = Utils::normal( _vertices[triangle2.vertex1], _vertices[triangle2.vertex2], _vertices[triangle2.vertex3] );
		}
	}

	// Calculate vertex normals
	for ( uint widthPoint = 0; widthPoint < _widthResolution; widthPoint++ )
	{
		for ( uint lengthPoint = 0; lengthPoint < _lengthResolution; lengthPoint++ )
		{
			int v = _widthResolution*lengthPoint + widthPoint;
			Vector3D &normal = _vertexNormals[v];
			normal = Vector3D( 0.0, 0.0, 0.0 );

			int t1 = ( widthPoint > 0 && lengthPoint > 0 ) ? ( 2*( lengthPoint - 1)*( _widthResolution - 1 ) + 2*( widthPoint - 1 ) ) : -1,
			t2 = ( widthPoint > 0 && lengthPoint > 0 ) ? ( t1 + 1 ) : -1,
			t3 = ( widthPoint < ( _widthResolution - 1 ) && lengthPoint > 0 ) ? ( 2*( lengthPoint - 1)*( _widthResolution - 1 ) + 2*( widthPoint ) + 1 ) : -1,
			t4 = ( widthPoint > 0 && lengthPoint < ( _lengthResolution - 1 ) ) ? ( 2*( lengthPoint )*( _widthResolution - 1 ) + 2*( widthPoint - 1 ) ) : -1,
			t5 = ( widthPoint < ( _widthResolution - 1 ) && lengthPoint < ( _lengthResolution - 1 ) ) ? ( 2*( lengthPoint )*( _widthResolution - 1 ) + 2*( widthPoint ) ) : -1,
			t6 = ( widthPoint < ( _widthResolution - 1 ) && lengthPoint < ( _lengthResolution - 1 ) ) ? ( t5 + 1 ) : -1;
			if ( t1 >= 0 ) normal += _triangleNormals[t1];
			if ( t2 >= 0 ) normal += _triangleNormals[t2];
			if ( t3 >= 0 ) normal += _triangleNormals[t3];
			if ( t4 >= 0 ) normal += _triangleNormals[t4];
			if ( t5 >= 0 ) normal += _triangleNormals[t5];
			if ( t6 >= 0 ) normal += _triangleNormals[t6];

			Utils::normalize( normal );
		}
	}

	buildDisplayLists();
}

Terrain::~Terrain()
{
	glDeleteLists( _displayLists, _numDisplayLists );
	delete _heightData;
	delete _texture;
}

void Terrain::buildDisplayLists()
{

	// Flat shading
	glNewList( _displayLists, GL_COMPILE );

	glShadeModel( GL_FLAT );

	_material.setActive();
	_texture->setActive( true );

	glBegin( GL_TRIANGLES );

	for ( uint triangleID = 0; triangleID < _triangles.size(); triangleID++ )
	{
		const Triangle &triangle = _triangles[triangleID];

		const Vector3D &triangleNormal = _triangleNormals[triangleID];
		glNormal3f(triangleNormal.x,triangleNormal.y, triangleNormal.z );

		const Point &vertex1 = _vertices[triangle.vertex1];
		const Point &texVertex1 = _textureCoordinates[triangle.vertex1];
		const Point &vertex2 = _vertices[triangle.vertex2];
		const Point &texVertex2 = _textureCoordinates[triangle.vertex2];
		const Point &vertex3 = _vertices[triangle.vertex3];
		const Point &texVertex3 = _textureCoordinates[triangle.vertex3];

		glTexCoord2f(texVertex1.x, texVertex1.y );
		glVertex3f( vertex1.x, vertex1.y, vertex1.z );

		glTexCoord2f( texVertex2.x, texVertex2.y);
		glVertex3f( vertex2.x, vertex2.y, vertex2.z );

		glTexCoord2f( texVertex3.x,texVertex3.y );
		glVertex3f( vertex3.x, vertex3.y, vertex3.z );
	}

	glEnd();
	_texture->setActive( false );

	glEndList();

	// Smooth shading
	glNewList( _displayLists + 1, GL_COMPILE );

	glShadeModel( GL_SMOOTH );

	_material.setActive();
	_texture->setActive( true );

	glBegin( GL_TRIANGLES );

	for ( uint triangleID = 0; triangleID < _triangles.size(); triangleID++ )
	{
		const Triangle &triangle = _triangles[triangleID];

		const Point &vertex1 = _vertices[triangle.vertex1];
		const Point &texVertex1 = _textureCoordinates[triangle.vertex1];
		const Point &vertex2 = _vertices[triangle.vertex2];
		const Point &texVertex2 = _textureCoordinates[triangle.vertex2];
		const Point &vertex3 = _vertices[triangle.vertex3];
		const Point &texVertex3 = _textureCoordinates[triangle.vertex3];

		const Vector3D &vertexNormal1 = _vertexNormals[triangle.vertex1];
		const Vector3D &vertexNormal2 = _vertexNormals[triangle.vertex2];
		const Vector3D &vertexNormal3 = _vertexNormals[triangle.vertex3];

		glNormal3f( vertexNormal1.x, vertexNormal1.y, vertexNormal1.z );
		glTexCoord2f(texVertex1.x,texVertex1.y);
		glVertex3f( vertex1.x, vertex1.y, vertex1.z );

		glNormal3f( vertexNormal2.x, vertexNormal2.y, vertexNormal2.z );
		glTexCoord2f(texVertex2.x,texVertex2.y);
		glVertex3f( vertex2.x, vertex2.y, vertex2.z );

		glNormal3f( vertexNormal3.x, vertexNormal3.y, vertexNormal3.z );
		glTexCoord2f(texVertex3.x,texVertex3.y);
		glVertex3f( vertex3.x, vertex3.y, vertex3.z );
	}

	glEnd();

	_texture->setActive( false );

	glEndList();
}

float Terrain::getHeight( const Point &point ) const
{
	uint indexOfNearestTriangle = getNearestTriangleIndexAt(point);

	Point p1(_vertices[_triangles[indexOfNearestTriangle].vertex1]);
	Point p2(_vertices[_triangles[indexOfNearestTriangle].vertex2]);
	Point p3(_vertices[_triangles[indexOfNearestTriangle].vertex3]);

	Vector3D normal = _triangleNormals[indexOfNearestTriangle];

	// formula to get the interpolated height of the three vertices
	return (normal.x * (point.x - p1.x) + normal.z * (point.z - p1.z)) / - normal.y + p1.y;
}

Vector3D Terrain::getNormal( const Point &point ) const
{
	uint indexOfNearestTriangle = getNearestTriangleIndexAt(point);

	Vector3D norm1 = _vertexNormals[_triangles[indexOfNearestTriangle].vertex1];
	Vector3D norm2 = _vertexNormals[_triangles[indexOfNearestTriangle].vertex2];
	Vector3D norm3 = _vertexNormals[_triangles[indexOfNearestTriangle].vertex3];

	float distance1 = Utils::distance(point, _vertices[_triangles[indexOfNearestTriangle].vertex1]);
	float distance2 = Utils::distance(point, _vertices[_triangles[indexOfNearestTriangle].vertex2]);
	float distance3 = Utils::distance(point, _vertices[_triangles[indexOfNearestTriangle].vertex3]);

	float tot_distance = distance1 + distance2 + distance3;

//	Vector3D interp = Vector3D( (1.0f - distance1/tot_distance) * norm1.x + (1.0f - distance2/tot_distance) * norm2.x + (1.0f - distance3/tot_distance) * norm3.x,
//		(1.0f - distance1/tot_distance) * norm1.y + (1.0f - distance2/tot_distance) * norm2.y + (1.0f - distance3/tot_distance) * norm3.y,
//		(1.0f - distance1/tot_distance) * norm1.z + (1.0f - distance2/tot_distance) * norm2.z + (1.0f - distance3/tot_distance) * norm3.z);
//
//	//Vector3D interp = Vector3D(norm1.x+norm2.x+norm3.x,norm1.y+norm2.y+norm3.y,norm1.z+norm2.z+norm3.z);
//	Utils::normalize(interp);

	//std::cout << "x: " << interp.x << " y: " << interp.y << " z: " << interp.z << "\n";
	return _triangleNormals[indexOfNearestTriangle];
}

void Terrain::draw() const
{
	glPolygonMode( GL_FRONT_AND_BACK, ( _renderingParameters.drawMode == RenderingParameters::WIREFRAME ) ? GL_LINE : GL_FILL );

	glEnable( GL_LIGHTING );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	glCallList( ( _renderingParameters.shadeMode == RenderingParameters::FLAT ) ? ( _displayLists ) : ( _displayLists + 1 ) );

	//draw the normals if they are toggled
	const float lineLength = 3.0f;
		if(_renderingParameters.normalMode == RenderingParameters::VERTEX)
		{
			for(uint vertexIndex = 0; vertexIndex < _numVertices; vertexIndex++)
			{
				glBegin(GL_LINES);
				glVertex3f(_vertices[vertexIndex].x,_vertices[vertexIndex].y,_vertices[vertexIndex].z);
				glVertex3f(_vertices[vertexIndex].x + _vertexNormals[vertexIndex].x*lineLength,_vertices[vertexIndex].y+ _vertexNormals[vertexIndex].y*lineLength,_vertices[vertexIndex].z+ _vertexNormals[vertexIndex].z*lineLength);
				glEnd();
			}
		}
		else if(_renderingParameters.normalMode == RenderingParameters::TRIANGLE)
		{
			for(uint triangleIndex = 0; triangleIndex < _numTriangles; triangleIndex++)
			{
				Triangle currentTriangle = _triangles[triangleIndex];
				Point baryPoint = Point( (_vertices[currentTriangle.vertex1].x + _vertices[currentTriangle.vertex2].x + _vertices[currentTriangle.vertex3].x) / 3.0f, (_vertices[currentTriangle.vertex1].y + _vertices[currentTriangle.vertex2].y + _vertices[currentTriangle.vertex3].y) / 3.0f, (_vertices[currentTriangle.vertex1].z + _vertices[currentTriangle.vertex2].z + _vertices[currentTriangle.vertex3].z) / 3.0f, 0);

				glBegin(GL_LINES);

				glVertex3f(baryPoint.x,baryPoint.y,baryPoint.z);
				glVertex3f(baryPoint.x + _triangleNormals[triangleIndex].x*lineLength,baryPoint.y+ _triangleNormals[triangleIndex].y*lineLength,baryPoint.z+ _triangleNormals[triangleIndex].z*lineLength);
				glEnd();
			}
		}

	glPopMatrix();
}



 uint Terrain::getNearestTriangleIndexAt( const Point &point) const{

	 float min_distance = 1000;
	 uint indexOfNearestTriangle = -1;

	 	for( uint triangleIndex = 0; triangleIndex < _vertices.size();triangleIndex++)
	 	{
	 		Point p1(_vertices[_triangles[triangleIndex].vertex1]);
	 		Point p2(_vertices[_triangles[triangleIndex].vertex2]);
	 		Point p3(_vertices[_triangles[triangleIndex].vertex3]);

	 		float distance = Utils::distance(point,Point((p1.x+p2.x+p3.x)/3.0f,(p1.y+p2.y+p3.y)/3.0f,(p1.z+p2.z+p3.z)/3.0f));
	 		if(distance < min_distance)
	 		{
	 			min_distance = distance;
	 			indexOfNearestTriangle = triangleIndex;
	 		}
	 	}
	 	return indexOfNearestTriangle;
}
GAME_NAMESPACE_END
