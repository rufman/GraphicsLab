/*
 * sceneryObjectManager.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef SCENERYOBJECTMANAGER_HPP_
#define SCENERYOBJECTMANAGER_HPP_

// Includes
#include "defs.hpp"
#include "drawable.hpp"
#include "map.hpp"
#include "tgaTexture.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

// Forward declarations
class Map;


class SceneryObjectManager {

public:

	/** Constructor */
	SceneryObjectManager(Map* map);

	/** Deconstructor */
	~SceneryObjectManager();

	/** Build display lists */
	void initDisplayLists();

	/** Deleted all scenery objects */
	void reset();

	/**	Adds a scenery object to the object vector */
	void addObject(Drawable* object);
	
	/** Get the collision value for e specific scenery object type */
	int getObjectCollisionType(int type);
	
	/** Draw a specific scenery object */
	void draw(int x, int y, float altitude, int type, int quality, float& animationPosition);

	/** Update duration value */
	void update(float duration);

private:
	Map* _map;

	float _duration;

	uint _treeDisplayListsNum;
	GLuint _treeDisplayLists;

	uint _grassDisplayListsNum;
	GLuint _grassDisplayLists;

	uint _bushDisplayListsNum;
	GLuint _bushDisplayLists;

	uint _stoneDisplayListsNum;
	GLuint _stoneDisplayLists;

	DrawableList _objects;

	TGATexture* _treeTexture;
	TGATexture* _grassBushTexture;
	TGATexture* _stoneTexture;
};


GRAPHICSLAB_NAMESPACE_END

#endif /* SCENERYOBJECTMANAGER_HPP_ */
