/*
 * sector.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */
#ifndef SECTOR_HPP_
#define SECTOR_HPP_

// Includes
#include "defs.hpp"
#include "drawable.hpp"
#include "tgaTexture.hpp"
#include "reflectionTexture.hpp"
#include "shadowTexture.hpp"
#include "map.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

// Forward declarations
class Map;


class Sector {

public:

	/** Constructor */
	Sector();

	/** Deconstructor */
	~Sector();

	/** Initialise Sector */
	void init(Map* map, int offsetX, int offsetY, int width, int height);

	/** Set height for point x, y in sector */
	void setHeight(int x, int y, float height);

	/** Set normal for point x, y in sector */
	void setNormal(int x, int y, vec3f normal);

	/** Set texture for point x, y in sector */
	void setTexture(int x, int y, uchar texture);

	/** Get height for point x, y in sector */
	float getHeight(int x, int y);

	/** Get point at x, y in sector */
	Point getPoint(int x, int y);

	/** Set color for point x, y in sector */
	void setColor(int x, int y, uchar red, uchar green, uchar blue);

	/** Get color for point x, y in sector */
	Color3f getColor(int x, int y);

	/** Initialise Display Lists */
	void initDisplayLists();

	/** Draw sector */
	void draw(int quality);

	/** Draw shadows overlay */
	void drawShadow(int quality);

	/** Draw water surface in sector */
	void drawWater(int quality, float waterAnimationPosition);

	/** Add object to sector */
	void addObject(Drawable* object);

	/** Remove object */
	void removeObject(Drawable* object);
	
	/** Get object*/
	Drawable* getObject(){return _objects.front();}

	/** Set surface Texture */
	void setSurfaceTexture(TGATexture* surfaceTexture);

	/** Set reflection Texture */
	void setReflectionTexture(ReflectionTexture* reflectionTexture);

	/** Set shadow Texture */
	void setShadowTexture(ShadowTexture* shadowTexture);

private:
	/** Draw curtain Vertices for Sector Curtains */
	void curtainVertices(int x, int y);

	static const int SURFACE_TEXTURE_SIZE = 64;

	Map* _map;

	int _offsetX;
	int _offsetY;
	int _width;
	int _height;

	bool _hasWater;

	uint _numDisplayLists;
	GLuint _displayLists;

	float* _heightMap;
	Color3f* _colorMap;
	vec3f* _normals;
	uchar* _texture;
	DrawableList _objects;

	TGATexture* _surfaceTexture;
	ReflectionTexture* _reflectionTexture;
	ShadowTexture* _shadowTexture;
};


GRAPHICSLAB_NAMESPACE_END

#endif /* SECTOR_HPP_ */
