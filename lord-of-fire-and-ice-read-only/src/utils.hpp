/**
 * utils.hpp
 * 
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_UTILS_HPP
#define GRAPHICSLAB_UTILS_HPP

// Includes
#include "defs.hpp"
#include "types.hpp"
#include "vector.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN

/** Class for some utility functions */
class Utils
{
public:
    /** PI number */
    static const float PI;
    
    /**
     * Convert degree angle to radian.
     *
     * @param[in]   degree  Angle in degrees
     *
     * @return      Angle in radian
     */
    static float toRadian( float degree );
    
    /**
     * Convert radian angle to degrees.
     *
     * @param[in]   radian  Angle in radians
     *
     * @return      Angle in degrees
     */
    static float toDegree( float radian );
    
    /**
     * Get the length of a vector.
     *
     * @param[in]   v       Vector to calculate the length of
     *
     * @return      Length of the given vector
     */
    static float length( const vec3f &v );
    
    /**
     * Normalize the given vector to size 1.0.
     *
     * @param[in]   v   Vector to normalize
     */
    static void normalize( vec3f &v );
    
    
    /**
     * Calculate the dot product of two vectors.
     *
     * @param[in]   v1      First vector
     * @param[in]   v2      Second vector
     *
     * @return      Dot product vector of two vectors
     */
    static float dot( const vec3f &v1, const vec3f &v2 );
    
    
    /**
     * Calculate the cross product of two vectors.
     *
     * @param[in]   v1      First vector
     * @param[in]   v2      Second vector
     *
     * @return      Cross product vector of two vectors
     */
    static vec3f cross( const vec3f &v1, const vec3f &v2 );
    
    /**
     * Calculate the normal of a triangle with given three points.
     *
     * @param[in]   p1      First vertex of the triangle
     * @param[in]   p2      Second vertex of the triangle
     * @param[in]   p3      Third vertex of the triangle
     *
     * @return      Normal of the triangle
     */
    static vec3f normal( const Point &p1, const Point &p2, const Point &p3 );
    
    
    /**
     * Calculate the normal of a triangle with given three points.
     *
     * @param[in]   v1      First vertex of the triangle
     * @param[in]   v2      Second vertex of the triangle
     * @param[in]   v3      Third vertex of the triangle
     *
     * @return      Normal of the triangle
     */
    static vec3f normal( const vec3f &v1, const vec3f &v2, const vec3f &v3 );


    static float getAzimuth(float fromX, float fromY, float toX, float toY);


    static float angleDifference(float alpha, float beta);


    static int abs(int value);


    static float abs(float value);


    static float distance(float x1, float y1, float x2, float y2);

    static vec3f worldToScreen(float worldX, float worldY, float worldZ);


    static void drawTexturedSphere(float radius, int slices, int stacks, float textureX, float textureY, float textureW, float textureH);

    static void drawTexturedCylinder(float radius1, float radius2, float height, int slices, float textureX, float textureY, float textureW, float textureH, bool covers);

    static void drawTexturedHalfCylinder(float radius1, float radius2, float height, int slices, float textureX, float textureY, float textureW, float textureH, bool covers);

    static uchar getByteOfFloat(int byte, float number);

    static float getFloatFromBytes(uchar byte0, uchar byte1, uchar byte2);

}; // class Utils

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_UTILS_HPP


