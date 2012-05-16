/*
 * dot.h
 */

#ifndef DOT_H_
#define DOT_H_

//common includes
#include "../../common/Drawable.hpp"
#include "../../common/GLIncludes.hpp"
#include "../../common/Material.hpp"

GAME_NAMESPACE_BEGIN

class Node: public Drawable
{
public:
	Node(Point position,Scene &scene);
	virtual ~Node();
	void draw() const;
	bool  equals(Node) const;

public:
	enum NodeState{
		FREE,
		OPEN,
		CLOSED,
		BLOCKED
	} _nodeState;

	enum PathState{
		NOTHING,
		STARTPOINT,
		ENDPOINT,
		PARTOFPATH
	} _pathState;

	Point _position;

	Material _freeMaterial;
	Material _openMaterial;
	Material _closedMaterial;
	Material _blockedMaterial;
	Material _startMaterial;
	Material _endMaterial;
	Material _partOfPathMaterial;

	Node* nextNode;
	float _g_score;
	float _h_score;
	float _f_score;
};

GAME_NAMESPACE_END

#endif /* DOT_H_ */
