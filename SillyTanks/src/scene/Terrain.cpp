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

	_heightData = new TGAImage(); _heightData->load( heightFieldFileName.str() );
	_texture = new TGATexture( textureFileName.str() );

	if ( _widthResolution == 0 ) _widthResolution = _heightData->getWidth();
	if ( _lengthResolution == 0 ) _lengthResolution = _heightData->getHeight();

	int numVertices = _widthResolution*_lengthResolution;
	for (int i=0;i<numVertices;i++) {
		_vertices.push_back(Point());
		_vertexNormals.push_back(Vector3D());
	}

	int numTriangles = 2*( _widthResolution - 1 )*( _lengthResolution - 1 );
	for (int i=0;i<numTriangles;i++) {
		_triangles.push_back(Triangle(0,0,0));
		_triangleNormals.push_back(Vector3D());
	}

	// Calculate vertices
	uint xSlice = _texture->getWidth()/_widthResolution;
	uint zSlice = _texture->getHeight()/_lengthResolution;

	for ( int widthPoint = 0; widthPoint < _widthResolution; widthPoint++ )
	{
		for ( int lengthPoint = 0; lengthPoint < _lengthResolution; lengthPoint++ )
		{
			Point &vertex = _vertices[_widthResolution*lengthPoint + widthPoint];
			vertex.x = -_width/2.0 + widthPoint*( _width/( _widthResolution - 1 ) );
			vertex.z = _length/2.0 - lengthPoint*( _length/( _lengthResolution - 1 ) );
			vertex.y = (_heightData->getData()[_heightData->getWidth()*(lengthPoint*zSlice)*4 + widthPoint*xSlice*4])/50.0f;
		}
	}

	// Build triangles
	for ( int widthPoint = 0; widthPoint < ( _widthResolution - 1 ); widthPoint++ )
	{
		for ( int lengthPoint = 0; lengthPoint < ( _lengthResolution - 1 ); lengthPoint++ )
		{
			int t1 = 2*lengthPoint*( _widthResolution - 1 ) + widthPoint*2;
			int t2 = t1 + 1;
			Triangle &triangle1 = _triangles[t1];
			Triangle &triangle2 = _triangles[t2];

			int v1 = ( lengthPoint )*_widthResolution + ( widthPoint );
			int v2 = v1 + 1;
			int v3 = v2 + _widthResolution;
			int v4 = v3 - 1;
			triangle1.vertex1 = v1; triangle1.vertex2 = v2; triangle1.vertex3 = v3;
			triangle2.vertex1 = v1; triangle2.vertex2 = v3; triangle2.vertex3 = v4;
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

			_triangleNormals[t1] = Utils::normal( _vertices[triangle1.vertex1], _vertices[triangle1.vertex2], _vertices[triangle1.vertex3] );
			_triangleNormals[t2] = Utils::normal( _vertices[triangle2.vertex1], _vertices[triangle2.vertex2], _vertices[triangle2.vertex3] );
		}
	}

	// Calculate vertex normals
	for ( int widthPoint = 0; widthPoint < _widthResolution; widthPoint++ )
	{
		for ( int lengthPoint = 0; lengthPoint < _lengthResolution; lengthPoint++ )
		{
			int v = _widthResolution*lengthPoint + widthPoint;
			Point &vertex = _vertices[v];
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
	uint xSlice = _texture->getWidth()/_widthResolution;
	uint zSlice = _texture->getHeight()/_lengthResolution;

	int numTriangles = 2*( _widthResolution - 1 )*( _lengthResolution - 1 );

	// Flat shading
	glNewList( _displayLists, GL_COMPILE );

	glShadeModel( GL_FLAT );

	_material.setActive();
	_texture->setActive( true );

	/////////////////////////////////////////////////////////////
	////////////////////// GRAPHICS LAB 07 //////////////////////
	/////////////////////////////////////////////////////////////
	// Replace code here to define the display list for flat shading.
	// Make sure the texture coordinates are applied.
	/////////////////////////////////////////////////////////////
	glBegin( GL_TRIANGLES );

	for ( int t = 0; t < numTriangles; t++ )
	{

		const Vector3D &normal = _triangleNormals[t];
		glNormal3f( normal.x, normal.y, normal.z );

		const Point &vertex1 = _vertices[_triangles[t].vertex1];
		const Point &vertex2 = _vertices[_triangles[t].vertex2];
		const Point &vertex3 = _vertices[_triangles[t].vertex3];

		glTexCoord2f( ((vertex1.x+_width/2.0)/( _width/( _widthResolution - 1 )))
				*xSlice, ((-vertex1.z+_length/2.0)/(_length/( _lengthResolution - 1 )))*zSlice);
		glVertex3f( vertex1.x, vertex1.y, vertex1.z );

		glTexCoord2f( ((vertex2.x+_width/2.0)/( _width/( _widthResolution - 1 )))
				*xSlice, ((-vertex2.z+_length/2.0)/(_length/( _lengthResolution - 1 )))*zSlice);
		glVertex3f( vertex2.x, vertex2.y, vertex2.z );

		glTexCoord2f( ((vertex3.x+_width/2.0)/( _width/( _widthResolution - 1 )))
				*xSlice, ((-vertex3.z+_length/2.0)/(_length/( _lengthResolution - 1 )))*zSlice);
		glVertex3f( vertex3.x, vertex3.y, vertex3.z );
	}

	glEnd();
	/////////////////////////////////////////////////////////////

	//_texture->setActive( false );

	glEndList();

	// Smooth shading
	glNewList( _displayLists + 1, GL_COMPILE );

	glShadeModel( GL_SMOOTH );

	_material.setActive();
	_texture->setActive( true );

	/////////////////////////////////////////////////////////////
	////////////////////// GRAPHICS LAB 07 //////////////////////
	/////////////////////////////////////////////////////////////
	// Replace code here to define the display list for smooth shading.
	// Make sure the texture coordinates are applied.
	/////////////////////////////////////////////////////////////
	glBegin( GL_TRIANGLES );

	for ( int t = 0; t < numTriangles; t++ )
	{

		const Point &vertex1 = _vertices[_triangles[t].vertex1];
		const Point &vertex2 = _vertices[_triangles[t].vertex2];
		const Point &vertex3 = _vertices[_triangles[t].vertex3];

		if ( _renderingParameters.shadeMode == RenderingParameters::SMOOTH )
		{
			const Vector3D &normal = _vertexNormals[_triangles[t].vertex1];
			glNormal3f( normal.x, normal.y, normal.z );
		}
		glTexCoord2f( ((vertex1.x+_width/2.0)/( _width/( _widthResolution - 1 )))
				*xSlice, ((-vertex1.z+_length/2.0)/(_length/( _lengthResolution - 1 )))*zSlice);
		glVertex3f( vertex1.x, vertex1.y, vertex1.z );

		if ( _renderingParameters.shadeMode == RenderingParameters::SMOOTH )
		{
			const Vector3D &normal = _vertexNormals[_triangles[t].vertex2];
			glNormal3f( normal.x, normal.y, normal.z );
		}
		glTexCoord2f( ((vertex2.x+_width/2.0)/( _width/( _widthResolution - 1 )))
				*xSlice, ((-vertex2.z+_length/2.0)/(_length/( _lengthResolution - 1 )))*zSlice);
		glVertex3f( vertex2.x, vertex2.y, vertex2.z );

		if ( _renderingParameters.shadeMode == RenderingParameters::SMOOTH )
		{
			const Vector3D &normal = _vertexNormals[_triangles[t].vertex3];
			glNormal3f( normal.x, normal.y, normal.z );
		}
		glTexCoord2f( ((vertex3.x+_width/2.0)/( _width/( _widthResolution - 1 )))
				*xSlice, ((-vertex3.z+_length/2.0)/(_length/( _lengthResolution - 1 )))*zSlice);
		glVertex3f( vertex3.x, vertex3.y, vertex3.z );
	}

	glEnd();
	/////////////////////////////////////////////////////////////

	//_texture->setActive( false );

	glEndList();
}

void Terrain::draw() const
{
	glPolygonMode( GL_FRONT_AND_BACK, ( _renderingParameters.drawMode == RenderingParameters::WIREFRAME ) ? GL_LINE : GL_FILL );

	glEnable( GL_LIGHTING );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	glCallList( ( _renderingParameters.shadeMode == RenderingParameters::FLAT ) ? ( _displayLists ) : ( _displayLists + 1 ) );

	glPopMatrix();
}

float Terrain::getHeight( const Point &point ) const
{
	//float y = (_triangleNormals[triangleNumber].x*point.x + _triangleNormals[triangleNumber].z*point.z)/_triangleNormals[triangleNumber].y;
	int triangleNumber = getNearestTriangleIndexAt(point);
	Point firstPoint = _triangles[triangleNumber].vertex1;
	Point secondPoint = _triangles[triangleNumber].vertex2;
	Point thirdPoint = _triangles[triangleNumber].vertex3;

	float dist1 = Utils::distance(point, _triangles[triangleNumber].vertex1);
	float dist2 = Utils::distance(point, _triangles[triangleNumber].vertex2);
	float dist3 = Utils::distance(point, _triangles[triangleNumber].vertex3);

	float totalDist=dist1+dist2+dist3;

	float ratio1 = dist1/totalDist;
	float ratio2 = dist2/totalDist;
	float ratio3 = dist3/totalDist;

	float height = (ratio1*firstPoint.y)
			+(ratio2*secondPoint.y)
			+(ratio3*thirdPoint.y);
	return height+2;
}

Vector3D Terrain::getNormal( const Point &point ) const
{
	int triangleNumber = getNearestTriangleIndexAt(point);
	//std::cout << "triangle number:" << triangleNumber<< std::endl;

	Vector3D norm1 = _vertexNormals[_triangles[triangleNumber].vertex1];
	Vector3D norm2 = _vertexNormals[_triangles[triangleNumber].vertex2];
	Vector3D norm3 = _vertexNormals[_triangles[triangleNumber].vertex3];

	float dist1 = Utils::distance(point, _triangles[triangleNumber].vertex1);
	float dist2 = Utils::distance(point, _triangles[triangleNumber].vertex2);
	float dist3 = Utils::distance(point, _triangles[triangleNumber].vertex3);

	float totalDist=dist1+dist2+dist3;

	float ratio1 = dist1/totalDist;
	float ratio2 = dist2/totalDist;
	float ratio3 = dist3/totalDist;

	//std::cout << "ratio1:" << ratio1<< std::endl;

	return Vector3D( norm1.x*ratio1+norm2.x*ratio2+norm3.x*ratio3 , norm1.y*ratio1+norm2.y*ratio2+norm3.y*ratio3, norm1.z*ratio1+norm2.z*ratio2+norm3.z*ratio3 );
}

int Terrain::getNearestTriangleIndexAt( const Point &point) const {
	/**
	 I'm doing something wrong here, but I cannot find the problem, I'm calculating somehow the wrong trianglenumber... :S But I don't want to bruteforce

	 **/
	float widthVertDistance = _width/(_widthResolution-1);
	float lengthVertDistance = _length/(_lengthResolution-1);

	float slope = lengthVertDistance/widthVertDistance;

	float columnNumber = (point.x+(_width/2))/widthVertDistance;
	float rowNumber = (-point.z+(_length/2))/lengthVertDistance;

	float restColumn = columnNumber - (int) columnNumber;
	float restRow = rowNumber - (int) rowNumber;

	int triangleNumber=1;
	if(restRow - slope*restColumn > 0) {
		//upper triangle
		triangleNumber = (int) rowNumber*((_widthResolution-1)*2)+((int) columnNumber)*2+1;

	} else {
		//lower triangle
		triangleNumber = (int) rowNumber*((_widthResolution-1)*2)+((int) columnNumber)*2;
	}

	//    std::cout << "triangle number:" << triangleNumber<< std::endl;
	//    std::cout << "column number:" << columnNumber<< std::endl;
	//    std::cout << "row number:" << rowNumber<< std::endl;
	//    std::cout << "restRow number:" << restRow<< std::endl;
	//    std::cout << "restColumn number:" << restColumn<< std::endl;
	return triangleNumber;
}
GAME_NAMESPACE_END
