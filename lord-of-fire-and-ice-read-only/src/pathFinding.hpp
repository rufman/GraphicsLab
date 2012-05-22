/**
 * pathFinding.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_PATHFINDING_HPP
#define GRAPHICSLAB_PATHFINDING_HPP

//Includes
#include "defs.hpp"
#include "types.hpp"
#include "map.hpp"
#include <cmath>
#include <queue>


GRAPHICSLAB_NAMESPACE_BEGIN

struct PathFindingNode {
	int x;
	int y;
	int g;
	int h;
	PathFindingNode* parentNode;
	bool open;
	PathFindingNode(int x_, int y_, int g_, int h_, PathFindingNode* parentNode_, bool open_) :
		x(x_),
		y(y_),
		g(g_),
		h(h_),
		parentNode(parentNode_),
		open(open_) {}
};


struct PathFindingFValue {
	int f;
	int x;
	int y;
	PathFindingFValue(int f_, int x_, int y_) :
		f(f_),
		x(x_),
		y(y_) {}
	PathFindingFValue() :
		f(0),
		x(0),
		y(0) {}
};


class PathFindingFValueComparator {
public:
	bool operator()(PathFindingFValue arg0, PathFindingFValue arg1) {
		if(arg0.f > arg1.f) {
			return true;
		} else if(arg0.f < arg1.f) {
			return false;
		}
		return false;
	}

};



class PathFinding {

public:

	PathFinding(Map* map, int npcId, float startX, float startY, float targetX, float targetY, float nodeDistance);

    ~PathFinding();

    PointList find();

    Point getDestination();

    
private:

    Map* _map;

    int _npcId;

	PathFindingNode*** _nodes;
	bool** _blockedNodes;
	std::priority_queue<PathFindingFValue, std::vector<PathFindingFValue>, PathFindingFValueComparator> _fValues;

	float _nodeDistance;
	static const int MIN_GRID_SIZE = 20;

	float _mapStartX;
	float _mapStartY;

	float _mapTargetX;
	float _mapTargetY;

	int _gridSizeX;
	int _gridSizeY;

	int _startX;
	int _startY;
	int _targetX;
	int _targetY;
	int _distance;

	float _ratioX;
	float _ratioY;

};

GRAPHICSLAB_NAMESPACE_END

#endif  //  GRAPHICSLAB_PATHFINDING_HPP





