/**
 * types.cpp
*
 */

// Class declaration include
#include "Types.hpp"


namespace game_space {


///////////////////////////////////////////////////////////////////
// Point
///////////////////////////////////////////////////////////////////
Point::Point( float x_, float y_, float z_, float w_ ) :
	x( _arr[0] ), y( _arr[1] ), z( _arr[2] ), w( _arr[3] )
{
	x = x_; y = y_; z = z_; w = w_;
}

Point::Point( const Point &other ) :
	x( _arr[0] ), y( _arr[1] ), z( _arr[2] ), w( _arr[3] )
{
	x = other.x; y = other.y; z = other.z; w = other.w;
}

void Point::operator=( const Point &other )
{
	x = other.x; y = other.y; z = other.z; w = other.w;
}

Point Point::operator-(Point &other ) const
{
	return Point(x-other.x,y-other.y,z-other.z,1);
}

const float *Point::operator()() const
{
	return _arr;
}

///////////////////////////////////////////////////////////////////
// Vector3D
///////////////////////////////////////////////////////////////////
Vector3D::Vector3D( float x_, float y_, float z_ ) :
	x( _arr[0] ), y( _arr[1] ), z( _arr[2] )
{
	x = x_; y = y_; z = z_;
}

Vector3D::Vector3D( const Vector3D &other ) :
	x( _arr[0] ), y( _arr[1] ), z( _arr[2] )
{
	x = other.x; y = other.y; z = other.z;
}

void Vector3D::operator=( const Vector3D &other )
{
	x = other.x; y = other.y; z = other.z;
}

const float *Vector3D::operator()() const
{
	return _arr;
}

///////////////////////////////////////////////////////////////////
// Color
///////////////////////////////////////////////////////////////////
const Color &Color::RED = Color( 1.0f, 0.0f, 0.0f );
const Color &Color::GREEN = Color( 0.0f, 1.0f, 0.0f );
const Color &Color::BLUE = Color( 0.0f, 0.0f, 1.0f );
const Color &Color::YELLOW = Color( 1.0f, 1.0f, 0.0f );
const Color &Color::GRAY = Color( 0.5f, 0.5f, 0.5f );
const Color &Color::BLACK = Color( 0.0f, 0.0f, 0.0f );
const Color &Color::WHITE = Color( 1.0f, 1.0f, 1.0f );
const Color &Color::SKY_BLUE = Color( 0.53f, 0.8f, 0.98f );
const Color &Color::SADDLE_BROWN = Color( 0.54f, 0.27f, 0.07f );
const Color &Color::TAN = Color( 0.82f, 0.70f, 0.55f );

Color::Color( float r_, float g_, float b_, float a_ ) :
	r( _arr[0] ), g( _arr[1] ), b( _arr[2] ), a( _arr[3] )
{
	r = r_; g = g_; b = b_; a = a_;
}

Color::Color( const Color &other ) :
	r( _arr[0] ), g( _arr[1] ), b( _arr[2] ), a( _arr[3] )
{
	r = other.r; g = other.g; b = other.b; a = other.a;
}

void Color::operator=( const Color &other )
{
	r = other.r; g = other.g; b = other.b; a = other.a;
}

const float *Color::operator()() const
{
	return _arr;
}


}
