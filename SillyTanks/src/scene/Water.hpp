/**
 * water.hpp
 * 
 */

#ifndef GRAPHICSLAB_WATER_HPP
#define GRAPHICSLAB_WATER_HPP

// could not remove them
#include "../common/Drawable.hpp"
#include "../common/ScreenTexture.hpp"
#include "../common/Material.hpp"

namespace game_space {

class Point;

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
	Water(Scene &scene, float waterHeight, float width, float length);

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

	float getHeightOffset() const;

	/**
	 * Get normal at a point.
	 *
	 * @param[in]	point	Point to get normal at
	 *
	 * @return		Normal value at given point
	 */
	Vector3D getNormal(const Point &point) const;

	/**
	 * Set up OpenGL camera to imitate mirror, as if
	 * there is a camera behind the mirror glass.
	 */
	void applyCamera() const;

	/**
	 * Capture the drawn image after applyCamera() is called as
	 * the mirror texture, which can be applied to the mirror surface rectangle.
	 */
	void capture();

	float waterHeightOffset;

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

	Material _material;

	ScreenTexture _texture;

};

}

#endif // GRAPHICSLAB_WATER_HPP
