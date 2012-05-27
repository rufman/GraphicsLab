/**
 * utils.hpp
* See utils.cpp
 */

#ifndef GRAPHICSLAB_UTILS_HPP
#define GRAPHICSLAB_UTILS_HPP


// Global definitions include
#include "Definitions.hpp"

// Includes
#include "Types.hpp"


namespace game_space {


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
	 * Get Euclidean distance between two points.
	 *
	 * @param[in]	p1		First point
	 * @param[in]	p2		Second point
	 *
	 * @return		Distance between two points
	 */
	static float distance( const Point &p1, const Point &p2 );
	
	 /**
     * Get the length of a vector.
     *
     * @param[in]   v       Vector to calculate the length of
     *
     * @return      Length of the given vector
     */
    static float length( const Vector3D &v );
    
    /**
     * Normalize the given vector to size 1.0.
     *
     * @param[in]   v   Vector to normalize
     */
    static void normalize( Vector3D &v );
    
    
    /**
     * Calculate the dot product of two vectors.
     *
     * @param[in]   v1      First vector
     * @param[in]   v2      Second vector
     *
     * @return      Dot product vector of two vectors
     */
    static float dot( const Vector3D &v1, const Vector3D &v2 );
    
    
    /**
     * Calculate the cross product of two vectors.
     *
     * @param[in]   v1      First vector
     * @param[in]   v2      Second vector
     *
     * @return      Cross product vector of two vectors
     */
    static Vector3D cross( const Vector3D &v1, const Vector3D &v2 );

	/**
     * Calculate the normal of a triangle with given three points.
     *
     * @param[in]   p1      First vertex of the triangle
     * @param[in]   p2      Second vertex of the triangle
     * @param[in]   p3      Third vertex of the triangle
     *
     * @return      Normal of the triangle
     */
    static Vector3D normal( const Point &p1, const Point &p2, const Point &p3 );
	
	/**
	 * Rotate a vector around another vector for given angle
	 *
	 * @param[in]	angle	Angle in degrees to rotate
	 * @param[in]	v		Vector to rotate
	 * @param[in]	axis	Rotation axis
	 *
	 * @return		Rotated vector
	 */
	static Vector3D rotate( float angle, const Vector3D &v, const Vector3D &axis );

	/**
	 * Apply OpenGL rotation to bring v1 to v2
	 *
	 * @param[in]	v1	Vector to rotate
	 * @param[in]	v2	Rotated vector
	 */
	static void applyGLRotation( const Vector3D &v1, const Vector3D &v2 );

	static float getElevation(Point startPosition,Point targetPosition,float velocity,bool mortarAim,float weight);

}; // class Utils


}


#endif // GRAPHICSLAB_UTILS_HPP
