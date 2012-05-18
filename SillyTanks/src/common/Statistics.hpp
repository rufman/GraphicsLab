/*
 * Statistics.hpp
 *
 *  Created on: 18.05.2012
 *      Author: gmazlami
 */

#ifndef STATISTICS_HPP_
#define STATISTICS_HPP_
#define MAX_ENEMIES 20


class Statistics {


public:

	//initialisation at the beginning
	static void init();

	//getters
	static float getLive();
	static int getEnemiesKilled();
	static int getEnemiesRemaining();


	//mutators
	static void decreaseLive();
	static void increaseLive();
	static void decreaseEnemies();


private:


	static float _live;   //can take values from 0.0 to 100.0 ;
	static int _enemiesKilled;
	static int _totalEnemies;



};

#endif /* STATISTICS_HPP_ */
