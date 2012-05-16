/**
 * utils.cpp
* Utilities used in this game, most of them are used for 3D graphics operations and measuring distances.
 */

// Class declaration include
#include "Utils.hpp"

// Includes
#include "GLIncludes.hpp"

#include <cmath>


GAME_NAMESPACE_BEGIN


const float Utils::PI = 3.14159265f;

float Utils::toRadian( float degree )
{
    return ( degree*PI/180.0f);
}

float Utils::toDegree( float radian )
{
    return ( radian*180.0f/PI );
}

float Utils::distance( const Point &p1, const Point &p2 )
{
	float xDiff = p2.x - p1.x;
	float yDiff = p2.y - p1.y;
	float zDiff = p2.z - p1.z;
	
	return std::sqrt( xDiff*xDiff + yDiff*yDiff + zDiff*zDiff );
}

float Utils::length( const Vector3D &v )
{
    return std::sqrt( v.x*v.x + v.y*v.y + v.z*v.z );
}

void Utils::normalize( Vector3D &v )
{
    float l = length( v );
	v /= l;
}

float Utils::dot( const Vector3D &v1, const Vector3D &v2 )
{
    return ( v1.x*v2.x + v1.y*v2.y + v1.z*v2.z );
}

Vector3D Utils::cross( const Vector3D &v1, const Vector3D &v2 )
{
   Vector3D result( v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x );
   // return Vector3D(v1.y*v2.z - v1.z*v2.y,v1.x*v2.z - v1.z*v2.x,v1.x*v2.y-v1.y*v2.x);
   return result;
}

Vector3D Utils::normal( const Point &p1, const Point &p2, const Point &p3 )
{
    Vector3D v1( p2.x - p1.x, p2.y - p1.y, p2.z - p1.z );
    Vector3D v2( p3.x - p1.x, p3.y - p1.y, p3.z - p1.z );
    
    Vector3D result = cross( v1, v2 );
    normalize( result );
    
    return result;
}

Vector3D Utils::rotate( float angle, const Vector3D &v, const Vector3D &axis )
{
	if ( angle == 0 )
		return Vector3D( v );
	
	float cosTheta = std::cos( toRadian( angle ) );
	float sinTheta = std::sin( toRadian( angle ) );
	
	Vector3D v_ = v; normalize( v_ );
	Vector3D axis_ = axis; normalize( axis_ );
	
	Vector3D result( 
		( ( 1.0 - cosTheta )*axis_.x*axis_.x + cosTheta )*v_.x + ( ( 1.0 - cosTheta )*axis_.x*axis_.y + sinTheta*axis_.z )*v_.y + ( ( 1.0 - cosTheta )*axis_.x*axis_.z - sinTheta*axis_.y )*v_.z,
		( ( 1.0 - cosTheta )*axis_.x*axis_.y - sinTheta*axis_.z )*v_.x + ( ( 1.0 - cosTheta )*axis_.y*axis_.y + cosTheta )*v_.y + ( ( 1.0 - cosTheta )*axis_.y*axis_.z + sinTheta*axis_.x )*v_.z,
		( ( 1.0 - cosTheta )*axis_.x*axis_.z + sinTheta*axis_.y )*v_.x + ( ( 1.0 - cosTheta )*axis_.y*axis_.z - sinTheta*axis_.x )*v_.y + ( ( 1.0 - cosTheta )*axis_.z*axis_.z + cosTheta )*v_.z
		);
	normalize( result );
	
	result *= length( v );
	
	return result;
}

void Utils::applyGLRotation( const Vector3D &v1, const Vector3D &v2 )
{	
	Vector3D vec1 = v1, vec2 = v2;
	normalize( vec1 ); normalize( vec2 );
	float angle = std::acos( Utils::dot( vec1, vec2 ) );
	if ( angle == 0 )
		return;
	
	Vector3D rotateAxis = Utils::cross( vec1, vec2 );
	Utils::normalize( rotateAxis );
	glRotatef( Utils::toDegree( angle ), rotateAxis.x, rotateAxis.y, rotateAxis.z );
}

void Utils::drawBox( float xLen, float yLen, float zLen, uint xRes, uint yRes, uint zRes )
{
	float xSlice = xLen/( xRes - 1 );
	float ySlice = yLen/( yRes - 1 );
	float zSlice = zLen/( zRes - 1 );
	
	glNormal3f( 0.0, 0.0, 1.0 );
	glBegin( GL_QUADS );
	for ( uint x = 0; x < ( xRes - 1 ); x++ )
	{
		for ( uint y = 0; y < ( yRes - 1 ); y++ )
		{
			glVertex3f( -xLen/2 + x*xSlice, -yLen/2 + y*ySlice, zLen/2 );
			glVertex3f( -xLen/2 + (x+1)*xSlice, -yLen/2 + y*ySlice, zLen/2 );
			glVertex3f( -xLen/2 + (x+1)*xSlice, -yLen/2 + (y+1)*ySlice, zLen/2 );
			glVertex3f( -xLen/2 + x*xSlice, -yLen/2 + (y+1)*ySlice, zLen/2 );
		}
	}
	glEnd();


	glNormal3f( 0.0, 0.0, -1.0 );
	glBegin( GL_QUADS );
	for ( uint x = 0; x < ( xRes - 1 ); x++ )
	{
		for ( uint y = 0; y < ( yRes - 1 ); y++ )
		{
			glVertex3f( xLen/2 - x*xSlice, -yLen/2 + y*ySlice, -zLen/2 );
			glVertex3f( xLen/2 - (x+1)*xSlice, -yLen/2 + y*ySlice, -zLen/2 );
			glVertex3f( xLen/2 - (x+1)*xSlice, -yLen/2 + (y+1)*ySlice, -zLen/2 );
			glVertex3f( xLen/2 - x*xSlice, -yLen/2 + (y+1)*ySlice, -zLen/2 );
		}
	}
	glEnd();

	glNormal3f( 1.0, 0.0, 0.0 );
	glBegin( GL_QUADS );
	for ( uint z = 0; z < ( zRes - 1 ); z++ )
	{
		for ( uint y = 0; y < ( yRes - 1 ); y++ )
		{
			glVertex3f( xLen/2, -yLen/2 + y*ySlice, zLen/2 - z*zSlice );
			glVertex3f( xLen/2, -yLen/2 + y*ySlice, zLen/2 - (z+1)*zSlice );
			glVertex3f( xLen/2, -yLen/2 + (y+1)*ySlice, zLen/2 - (z+1)*zSlice );
			glVertex3f( xLen/2, -yLen/2 + (y+1)*ySlice, zLen/2 - z*zSlice );
		}
	}
	glEnd();

	glNormal3f( -1.0, 0.0, 0.0 );
	glBegin( GL_QUADS );
	for ( uint z = 0; z < ( zRes - 1 ); z++ )
	{
		for ( uint y = 0; y < ( yRes - 1 ); y++ )
		{
			glVertex3f( -xLen/2, -yLen/2 + y*ySlice, -zLen/2 + z*zSlice );
			glVertex3f( -xLen/2, -yLen/2 + y*ySlice, -zLen/2 + (z+1)*zSlice );
			glVertex3f( -xLen/2, -yLen/2 + (y+1)*ySlice, -zLen/2 + (z+1)*zSlice );
			glVertex3f( -xLen/2, -yLen/2 + (y+1)*ySlice, -zLen/2 + z*zSlice );
		}
	}
	glEnd();

	glNormal3f( 0.0, 1.0, 0.0 );
	glBegin( GL_QUADS );
	for ( uint x = 0; x < ( xRes - 1 ); x++ )
	{
		for ( uint z = 0; z < ( zRes - 1 ); z++ )
		{
			glVertex3f( -xLen/2 + x*xSlice, yLen/2, zLen/2 - z*zSlice );
			glVertex3f( -xLen/2 + (x+1)*xSlice, yLen/2, zLen/2 - z*zSlice );
			glVertex3f( -xLen/2 + (x+1)*xSlice, yLen/2, zLen/2 - (z+1)*zSlice );
			glVertex3f( -xLen/2 + x*xSlice, yLen/2, zLen/2 - (z+1)*zSlice );
		}
	}
	glEnd();
	
	glNormal3f( 0.0, -1.0, 0.0 );
	glBegin( GL_QUADS );
	for ( uint x = 0; x < ( xRes - 1 ); x++ )
	{
		for ( uint z = 0; z < ( zRes - 1 ); z++ )
		{
			glVertex3f( xLen/2 - x*xSlice, -yLen/2, -zLen/2 + z*zSlice );
			glVertex3f( xLen/2 - (x+1)*xSlice, -yLen/2, -zLen/2 + z*zSlice );
			glVertex3f( xLen/2 - (x+1)*xSlice, -yLen/2, -zLen/2 + (z+1)*zSlice );
			glVertex3f( xLen/2 - x*xSlice, -yLen/2, -zLen/2 + (z+1)*zSlice );
		}
	}
	glEnd();
}


GAME_NAMESPACE_END
