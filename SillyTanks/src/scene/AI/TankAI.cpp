/*
 * TankAI.cpp
*
 */

#include "TankAI.hpp"

namespace game_space {

TankAI::TankAI():
_strategy(TankAI::EXPLORE){

}

TankAI::~TankAI() {
}

void TankAI::brainTick()
{

}

void TankAI::switchStrategy(enum TANKAI_STRATEGY newStrategy,int target)
{

}

int TankAI::pickTarget()
{
	return -1;
}

void TankAI::sense()
{

}

void TankAI::explore()
{

}

void TankAI::hunt()
{

}

void TankAI::reinforcement()
{

}

void TankAI::rampage()
{

}

void TankAI::escape()
{

}
///////////////////////
//helper methods
//////////////////////

void TankAI::followPath()
{

}

void TankAI::aimAndFire(int target)
{

}



}
