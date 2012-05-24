/*
 * Projectile.cpp
 */

#include "Projectile.hpp"

namespace game_space {
Projectile::Projectile(Scene &scene,ProjectileType projectileType,int projectileOwnerId):Drawable(scene),_projectileType(projectileType),_projectileOwnerID(projectileOwnerId){}


void Projectile::draw() const
{

}


Point Projectile::getPosition(){


	return  _position;

	//FIXME: _position is never set or initialized...
}
}




