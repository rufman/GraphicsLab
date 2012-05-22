/**
 * types.hpp
 * 
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */


#ifndef GRAPHICSLAB_TYPES_HPP
#define GRAPHICSLAB_TYPES_HPP

#include "defs.hpp"
#include <vector>


GRAPHICSLAB_NAMESPACE_BEGIN

/** Structure for a 3D point */
struct Point
{
    float x, y, z, w;
    
    Point( float x_ = 0.0, float y_ = 0.0, float z_ = 0.0, float w_ = 1.0 ) :
        x( x_ ), y( y_ ), z( z_ ), w( w_ )
    {
    }
};


/** Structure for color */
struct Color
{
    float r, g, b, a;
    
    Color( float r_ = 0.0, float g_ = 0.0, float b_ = 0.0, float a_ = 1.0 ) :
        r( r_ ), g( g_ ), b( b_ ), a( a_ )
    {
    }
};

struct Color4f
{
    float r, g, b, a;

    Color4f( float r_ = 0.0, float g_ = 0.0, float b_ = 0.0, float a_ = 1.0 ) :
        r( r_ ), g( g_ ), b( b_ ), a( a_ )
    {
    }
};

struct Color3f
{
    float r, g, b;

    Color3f( float r_ = 0.0, float g_ = 0.0, float b_ = 0.0 ) :
        r( r_ ), g( g_ ), b( b_ )
    {
    }
};

struct Color3uc
{
    uchar r, g, b;

    Color3uc( uchar r_ = 0, uchar g_ = 0, uchar b_ = 0 ) :
        r( r_ ), g( g_ ), b( b_ )
    {
    }
};

struct Rectangle
{
    float x, y, width, height;
    
    Rectangle( float x_ = 0.0, float y_ = 0.0, float width_ = 1.0, float height_ = 1.0 ) :
        x( x_ ), y( y_ ), width( width_ ), height( height_ )
    {
    }
};

typedef std::vector <Point> PointList;

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_TYPES_HPP


