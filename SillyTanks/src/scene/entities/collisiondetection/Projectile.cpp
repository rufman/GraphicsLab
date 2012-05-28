/*
 * Projectile.cpp
 */

#include "Projectile.hpp"
#include "Target.hpp"

namespace game_space {
Projectile::Projectile(Scene& scene,ProjectileType projectileType,Target* projectileOwner) :
		Drawable(scene), _projectileType(projectileType), _projectileOwner(projectileOwner),_position(Point(0,0,0)),_detonated(false) {}

Projectile::~Projectile(){}

void Projectile::draw() const {}

void Projectile::setPosition(Point position) {
	_position = position;
}

const Point Projectile::getPosition() const {
	return _position;
}

}

