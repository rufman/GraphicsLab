/*
 * npc.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef NPC_HPP_
#define NPC_HPP_


// Includes
#include "defs.hpp"
#include "drawable.hpp"
#include "map.hpp"
#include "scene.hpp"
#include "player.hpp"
#include "pathFinding.hpp"
#include "types.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

// Forward declarations
class Map;
class NPC;
class Player;


class DrawableNPC : public Drawable {
public:
	DrawableNPC(Scene& scene, NPC* npc);
	void draw(int quality);
private:
	NPC* _npc;
};


class NPC : Drawable {

public:

	/** Constructor */
	NPC(Scene& scene, Map* map, Player* player);

	/** Deconstructor */
	~NPC();

	/** Initialise NPC */
	static void init();

	/** Assign ID to NPC */
	void assignID(int id);

	/** Get position of NPC */
	vec3f getPosition();

	/** Get direction of NPC */
	float getDirection();

	/** Set spawning Position of NPC */
	void setSpawnPosition(float x, float y);

	/** Set spawning Direction of NPC */
	void setSpawnDirection(float direction);

	/** Spawn NPC */
	void spawn();

	/** Place NPC at position */
	void place(float x, float y, float direction);

	/** Update NPC */
	void update(float duration);
	void moveTo(float x, float y);
	void passNode();

	/** Draw NPC */
	void draw(int quality);

	/** Remove NPC from Map */
	void remove();

	/** Set restlessness */
	void setRestlessness(float restlessness);

	/** Set if NPC is walking or running */
	void setLocomotionMode(bool);

	void setTShirtColor(float red, float green, float blue);

private:
	void updateSectorID();


	static const float WALKING_SPEED = 1.5;
	static const float RUNNING_SPEED = 4.5;
	static const float ACCELERATION = 20.0;

	static TGATexture* _npcTexture;

	Map* _map;
	Player* _player;

	int _sectorID;

	float _spawnX;
	float _spawnY;
	float _spawnDirection;
	float _oriX;
	float _oriY;
	float _nxtX;
	float _nxtY;
	float _dstX;
	float _dstY;

	float _speed;
	float _direction;
	bool _walking;
	float _walkingAnimationPosition;

	float _restlessness;
	float _restlessnessRating;

	DrawableNPC* _drawable;
	int _id;

	Color3f _tShirtColor;

	PointList _path;

	static uint _numDisplayLists;
	static GLuint _displayLists;
};

GRAPHICSLAB_NAMESPACE_END

#endif /* NPC_HPP_ */
