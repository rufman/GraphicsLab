/*
 * Projectile.hpp
 */

#ifndef PROJECTILE_HPP_
#define PROJECTILE_HPP_

#include "BoundingBox.hpp"
#include <vector>

namespace game_space {

class Projectile {
public:
	//this is necessary for the AI to know what tank took a shot on it.
	int _projectileOwnerID;
	std::vector<BoundingBox> _boundingBoxes;
public:
	Projectile(int projectileOwnerID) :
			_projectileOwnerID(projectileOwnerID) {
	}

	//a projectile has a bounding radius to first check if the projectile is even near to the target
	//(check if distance between projectile and target is higher than bounding radiuses added up)
	float getBoundingRadius() const;
	void addBoundingBox(BoundingBox box) {
		_boundingBoxes.push_back(box);
	}
	std::vector<BoundingBox> getBoundingBoxes() const;
};

}

#endif
