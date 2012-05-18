/**
 * types.hpp
*
 */

#ifndef GRAPHICSLAB_TYPES_HPP
#define GRAPHICSLAB_TYPES_HPP


// Global definitions include
#include "Definitions.hpp"


namespace game_space {


// typedef for basic types
typedef unsigned int uint;
typedef unsigned char uchar;

/** Structure for a 3D point */
struct Point
{
    float &x, &y, &z, &w;
    
    Point( float x_ = 0.0, float y_ = 0.0, float z_ = 0.0, float w_ = 1.0 );
	
	Point( const Point &other );
	void operator=( const Point &other );
	Point operator-(Point &other) const;

	/** Get the array */
	const float *operator()() const;
	
private:
	float _arr[4];
}; // struct Point

/** Structure for a 3D vector */
struct Vector3D
{
    float &x, &y, &z;
    
    Vector3D( float x_ = 0.0, float y_ = 0.0, float z_ = 0.0 );
	
	Vector3D( const Vector3D &other );
	void operator=( const Vector3D &other );
	
	/** Get the array */
	const float *operator()() const;
	
	
	Vector3D operator-()
	{
		return Vector3D( -x, -y, -z );
	}
	
	void operator+=( const Vector3D &v )
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}
	void operator-=( const Vector3D &v )
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;		
	}

	void operator*=( float s )
	{
		x *= s;
		y *= s;
		z *= s;
	}
	
	void operator/=( float s )
	{
		x /= s;
		y /= s;
		z /= s;
	}
	
	Vector3D operator+( const Vector3D &other ) const
	{
		return Vector3D( x + other.x, y + other.y, z + other.z );
	}

	Vector3D operator-( const Vector3D &other ) const
	{
		return Vector3D( x - other.x, y - other.y, z - other.z );
	}
	
private:
	float _arr[3];
}; // struct Vector3D

/** Structure for color */
struct Color
{
    float &r, &g, &b, &a;
    
    Color( float r_ = 0.0, float g_ = 0.0, float b_ = 0.0, float a_ = 1.0 );
	
	Color( const Color &other );
	void operator=( const Color &other );
	
	/** Get the array */
	const float *operator()() const;
    
    static const Color &RED;
    static const Color &GREEN;
    static const Color &BLUE;
    static const Color &YELLOW;
    static const Color &GRAY;
    static const Color &BLACK;
    static const Color &WHITE;
	static const Color &SKY_BLUE;
	static const Color &SADDLE_BROWN;
	static const Color &TAN;

private:
	float _arr[4];
}; // struct Color

/** Rendering parameters */
struct RenderingParameters
{
    enum DrawMode 
    {
        WIREFRAME,
        POLYGON
    } drawMode;
    
    enum ShadeMode
    {
        FLAT,
        SMOOTH
    } shadeMode;

    enum NormalMode
    	{
    		OFF,
    		VERTEX,
    		TRIANGLE
    	} normalMode;
	    
    RenderingParameters() :
        drawMode( POLYGON ),
        shadeMode( SMOOTH )
    {
    }
}; // struct RenderingParameters


}


#endif // GRAPHICSLAB_TYPES_HPP
