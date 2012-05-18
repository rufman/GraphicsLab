/*
 * Statistics.cpp
 *
 *  Created on: 18.05.2012
 *      Author: gmazlami
 */

#include "Statistics.hpp"


float Statistics::_live = 0.0;
int Statistics::_enemiesKilled  = 0 ;
int Statistics::_totalEnemies = 0 ;


void Statistics::init(){

	_live = 100.0;
	_totalEnemies = MAX_ENEMIES;
	_enemiesKilled = 0;
}


float Statistics::getLive(){
	return _live;
}

int Statistics::getEnemiesKilled(){
	return _enemiesKilled;
}

int Statistics::getEnemiesRemaining(){
	return (_totalEnemies - _enemiesKilled);
}

void Statistics::decreaseLive(){
	_live = _live - 1;
}

void Statistics::increaseLive(){
	_live = _live + 1;
}

void Statistics::decreaseEnemies(){
	_enemiesKilled++;
}

