/*
 * map.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */
#ifndef MAP_HPP_
#define MAP_HPP_

// Includes
#include "defs.hpp"
#include "drawable.hpp"
#include "sector.hpp"
#include "player.hpp"
#include "tgaTexture.hpp"
#include "sceneryObjectManager.hpp"
#include "reflectionTexture.hpp"
#include "shadowTexture.hpp"
#include <sstream>

GRAPHICSLAB_NAMESPACE_BEGIN

// Forward declarations
class Player;
class SceneryObjectManager;
class Sector;


class Map {

public:

	/** Constructor */
	Map(Scene& scene);

	/** Deconstructor */
	~Map();

	/** Load map from files */
	bool load(std::string directory);

	/** Get height for point x, y */
	float getHeight(int x, int y);

	/** Get point at x, y */
	Point getPoint(int x, int y);

	/** Set color for point x, y */
	void setColor(int x, int y, uchar red, uchar green, uchar blue);

	/** Get color for point x, y */
	Color3f getColor(int x, int y);

	/** Get sector which x, y is in */
	Sector* getSectorF(float x, float y);
	Sector* getSectorI(int x, int y);

	/** Get ID of sector which x, y is in */
	int getSectorID(float x, float y);

	/** Get sector with ID */
	Sector* getSectorByID(int id);

	/** Draw map */
	void draw(Player* _player);

	/** Draw reflected map for water-texture*/
	void drawReflected(Player* _player);

	/** Add object to map */
	void addObject(Drawable* object);

	/** Remove Object from map */
	void removeObject(Drawable* object);

	/** Get height of surface at point x, y */
	float getSurfaceHeight(float x, float y);

	/** Get the random number defined for x, y */
	int getRandom(int x, int y);

	/** Get map height */
	int getMapHeight();

	/** Get map width */
	int getMapWidth();
	
	/** Get obstructing Object at x, y */
	int getObstructingObjectI(int x, int y);

	/** Get obstructing Object at x, y */
	int getObstructingObjectF(float x_, float y_);

	/** Set obstructing Object at x, y */
	void setObstructingObjectI(int x, int y, int id);

	/** Set obstructing Object at x, y */
	void setObstructingObjectF(float x_, float y_, int id);

	/** Update map */
	void update(float duration);

	/** Get next sector corner relative to x and y */
	static vec3f getNextSectorCorner(float x, float y);

	/** get reflection from screen buffer */
	void grabReflectionTexture();

	/** get shadows from screen buffer */
	void grabShadowTexture();

	/** apply and remove reflection transformation */
	void applyReflectionTransformation();
	void removeReflectionTransformation();

	/** apply and remove shadow transformation */
	static void applyShadowTransformation();
	static void removeShadowTransformation();

	static const int LEVEL_OF_WATER = 2;
	static const float VERTICAL_SCALE = 0.2;

private:

	Scene& _scene;

	/** Set only height and not normal for point x, y */
	void setHeight(int x, int y, float height);

	/** Set normal for point x, y */
	void setNormal(int x, int y, vec3f normal);

	/** Set normal for point x, y */
	void setTexture(int x, int y, uchar texture);

	static const int SECTOR_SIZE = 16; // HAS TO BE A MULTIPLE OF 16;

    int _width;
    int _height;

    int _sectorsInX;
    int _sectorsInY;

    float _waterAnimationPosition;

    Sector* _sectors;

    TGATexture* _surfaceTexture;
    ReflectionTexture* _reflectionTexture;
    ShadowTexture* _shadowTexture;
    SceneryObjectManager* _sceneryObjectManager;

    int* _randomNumbers;
	
	int* _obstructingObjects;
};


GRAPHICSLAB_NAMESPACE_END

#endif /* MAP_HPP_ */
