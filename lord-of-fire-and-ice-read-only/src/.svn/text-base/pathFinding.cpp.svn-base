/**
 * pathFinding.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */


// Class declaration include
#include "pathFinding.hpp"

// Includes
#include <cmath>


GRAPHICSLAB_NAMESPACE_BEGIN


PathFinding::PathFinding(Map* map, int npcId, float startX, float startY, float targetX, float targetY, float nodeDistance) :
	_map(map),
	_npcId(npcId),
	_mapStartX(startX),
	_mapStartY(startY),
	_mapTargetX(targetX),
	_mapTargetY(targetY),
	_nodeDistance(nodeDistance) {

	float diffX = _mapTargetX - _mapStartX;
	float diffY = _mapTargetY - _mapStartY;

	float mapDistance = sqrt((diffX * diffX) + (diffY * diffY));

	if(mapDistance > 0.0) {
		_ratioX = diffX / mapDistance;
		_ratioY = diffY / mapDistance;
	} else {
		_ratioX = 0.0;
		_ratioY = 0.0;
	}

	_distance = round(mapDistance / _nodeDistance);

	float mapDiagonal = sqrt((_map->getMapWidth() * _map->getMapWidth()) + (_map->getMapHeight() * _map->getMapHeight()));
	int maxGridSize = (int) ceil(mapDiagonal / nodeDistance);

	_gridSizeX = _distance * 3;
	_gridSizeY = _distance * 2;

	if(_gridSizeX < MIN_GRID_SIZE) {
		_gridSizeX = MIN_GRID_SIZE;
	}
	if(_gridSizeY < MIN_GRID_SIZE) {
		_gridSizeY = MIN_GRID_SIZE;
	}
	if(_gridSizeX > maxGridSize) {
		_gridSizeX = maxGridSize;
	}
	if(_gridSizeY > maxGridSize) {
		_gridSizeY = maxGridSize;
	}

	_startX = (((_gridSizeX - _distance) / 2) - 1);
	_startY = (_gridSizeY / 2) - 1;
	_targetX = (((_gridSizeX - _distance) / 2) - 1) + _distance;
	_targetY = (_gridSizeY / 2) - 1;

	_nodes = new PathFindingNode**[_gridSizeX];
	_blockedNodes = new bool*[_gridSizeX];

	for(int x = 0; x < _gridSizeX; x++) {
		_nodes[x] = new PathFindingNode*[_gridSizeY];
		_blockedNodes[x] = new bool[_gridSizeY];
		for(int y = 0; y < _gridSizeY; y++) {
			_nodes[x][y] = NULL;
			_blockedNodes[x][y] = false;
		}
	}

	_fValues = std::priority_queue<PathFindingFValue, std::vector<PathFindingFValue>, PathFindingFValueComparator>();

}


PathFinding::~PathFinding() {
	for(int x = 0; x < _gridSizeX; x++) {
		for(int y = 0; y < _gridSizeY; y++) {
			delete _nodes[x][y];
		}
		delete [] _nodes[x];
		delete [] _blockedNodes[x];
	}

	delete [] _nodes;
	delete [] _blockedNodes;
}


PointList PathFinding::find() {

	int moveX[8] = { 0,  1,  1,  1,  0, -1, -1, -1};
	int moveY[8] = {-1, -1,  0,  1,  1,  1,  0, -1};
	int moveG[8] = {10, 14, 10, 14, 10, 14, 10, 14};

	PathFindingNode* targetNode = NULL;
	PathFindingFValue current;
	int newNodeX = 0;
	int newNodeY = 0;
	int gValue = 0;
	int hValue = 0;
	int xDistance = 0;
	int yDistance = 0;
	float tempX = 0;
	float tempY = 0;
	int obstruction = 0;


	_nodes[_startX][_startY] = new PathFindingNode(_startX, _startY, 0, 10 * _distance, NULL, true);
	_fValues.push(PathFindingFValue(10 * _distance, _startX, _startY));
	if(_startX == _targetX && _startY == _targetY) {
		targetNode = _nodes[_startX][_startY];
	}
	while(targetNode == NULL && !_fValues.empty()) {
		current = _fValues.top();
		_fValues.pop();
		_nodes[current.x][current.y]->open = false;

		for(int direction = 0; (direction < 8 && targetNode == NULL); direction++) {
			newNodeX = current.x + moveX[direction];
			newNodeY = current.y + moveY[direction];

			if(   newNodeX >= 0
			   && newNodeX < _gridSizeX
			   && newNodeY >= 0
			   && newNodeY < _gridSizeY
			   && !_blockedNodes[newNodeX][newNodeY]) {

				tempX = _mapStartX + ((((newNodeX - _startX) * _ratioX) - ((newNodeY - _startY) * _ratioY)) * _nodeDistance);
				tempY = _mapStartY + ((((newNodeX - _startX) * _ratioY) + ((newNodeY - _startY) * _ratioX)) * _nodeDistance);
				obstruction = _map->getObstructingObjectF(tempX, tempY);

				if(obstruction != 0 && obstruction != _npcId) {
					_blockedNodes[newNodeX][newNodeY] = true;
				} else {

					gValue = _nodes[current.x][current.y]->g + moveG[direction];

					if(_nodes[newNodeX][newNodeY] == NULL || _nodes[newNodeX][newNodeY]->open) {

						if(_nodes[newNodeX][newNodeY] != NULL) {

							if(_nodes[newNodeX][newNodeY]->g > gValue) {
								_nodes[newNodeX][newNodeY]->g          = gValue;
								_nodes[newNodeX][newNodeY]->parentNode = _nodes[current.x][current.y];
							}

						} else {

							xDistance = abs(newNodeX - _targetX);
							yDistance = abs(newNodeY - _targetY);
							if(xDistance > yDistance) {
							     hValue = (14 * yDistance) + (10 * (xDistance - yDistance));
							} else {
							     hValue = (14 * xDistance) + (10 * (yDistance - xDistance));
							}

							_nodes[newNodeX][newNodeY] = new PathFindingNode(newNodeX, newNodeY, gValue, hValue, _nodes[current.x][current.y], true);
							_fValues.push(PathFindingFValue(gValue + hValue, newNodeX, newNodeY));

							if(newNodeX == _targetX && newNodeY == _targetY) {
								targetNode = _nodes[newNodeX][newNodeY];
							}
						}
					}
				}
			}
		}
	}
	if(targetNode == NULL) {
		return PointList();
	}

	PointList wayPoints = PointList();
	wayPoints.push_back(Point(_mapTargetX, 0.0, _mapTargetY));

	PathFindingNode* wayPoint = targetNode->parentNode;

	while(wayPoint != NULL) {
		tempX = _mapStartX + ((((wayPoint->x - _startX) * _ratioX) - ((wayPoint->y - _startY) * _ratioY)) * _nodeDistance);
		tempY = _mapStartY + ((((wayPoint->x - _startX) * _ratioY) + ((wayPoint->y - _startY) * _ratioX)) * _nodeDistance);

		wayPoints.push_back(Point(tempX, 0.0, tempY));
		wayPoint = wayPoint->parentNode;
	}
	return wayPoints;
}


Point PathFinding::getDestination() {
	return Point(_mapTargetX, 0.0, _mapTargetY);
}


GRAPHICSLAB_NAMESPACE_END


