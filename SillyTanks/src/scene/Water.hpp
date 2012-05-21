/**
 * water.hpp
 * 
 */

#ifndef GRAPHICSLAB_WATER_HPP
#define GRAPHICSLAB_WATER_HPP

// commons includes
#include "../common/Drawable.hpp"
#include "../common/Types.hpp"

namespace game_space {

class TGATexture;

/** Water */
class Water: public Drawable {
public:
	/**
	 * Constructor
	 *
	 * @param[in, out]	scene	Owning scene
	 * @param[in]		width	Width of the sea
	 * @param[in]		length	Length of the sea
	 */
	Water(Scene &scene, float width, float length);

	/** Destructor */
	~Water();

	/** Draw the object to the screen. */
	void draw() const;

	/** Reset the sea parameters */
	void reset();

	/** Update sea surface for given duration */
	void update(float seconds);

	/**
	 * Get height at a point.
	 *
	 * @param[in]	point	Point to get height at
	 *
	 * @return		Height value at given point
	 */
	float getHeight(const Point &point) const;

	/**
	 * Get normal at a point.
	 *
	 * @param[in]	point	Point to get normal at
	 *
	 * @return		Normal value at given point
	 */
	Vector3D getNormal(const Point &point) const;

private:
	/** Calculate vertex heights */
	void calculateVertexHeights();

	/** Calculate vertex and triangle normals */
	void calculateNormals();

private:
	/** Displacement for updates */
	float _displacement;

	/** Dimensions of the sea */
	float _width, _length;

	/** Resolution of the sea mesh */
	int _widthResolution, _lengthResolution;

	/** List of vertices */
	Point *_vertices;

	/** Averaged normals of the vertices */
	Vector3D *_vertexNormals;

	struct Triangle {
		int vertex1, vertex2, vertex3;
	};
	/** List of triangles of the mesh */
	Triangle *_triangles;

	/** Normals of the triangles */
	Vector3D *_triangleNormals;

};

}

#endif // GRAPHICSLAB_WATER_HPP