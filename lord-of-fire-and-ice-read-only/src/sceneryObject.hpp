/*
 * sceneryObject.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef SCENERYOBJECT_HPP_
#define SCENERYOBJECT_HPP_

// Includes
#include "defs.hpp"
#include "drawable.hpp"
#include "sceneryObjectManager.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN


class SceneryObject : public Drawable {

public:

	/** Constructor */
	SceneryObject(Scene& scene, SceneryObjectManager* manager, int x, int y, float altitude, int type);

	/** Destructor */
	~SceneryObject();

	/** Draw sector */
	void draw(int quality);

private:
	SceneryObjectManager* _manager;
	int _type;
	float _animationPosition;
};


GRAPHICSLAB_NAMESPACE_END

#endif /* SCENERYOBJECT_HPP_ */
