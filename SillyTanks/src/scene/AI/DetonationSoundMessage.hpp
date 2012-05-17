/*
 * DetonationSoundMessage.h
 *
 *  Created on: May 17, 2012
 *      Author: leviathan
 */

#ifndef DETONATIONSOUNDMESSAGE_H_
#define DETONATIONSOUNDMESSAGE_H_

#include "Message.hpp"
#include "../../common/Types.hpp"

namespace game_space {

class DetonationSoundMessage: public game_space::Message {
public:
	Point _detonationPoint;
	float _detonationStrength;
public:
	DetonationSoundMessage(Point detonationPoint,float detonationStrength);
	virtual ~DetonationSoundMessage();
};

} /* namespace game_space */
#endif /* DETONATIONSOUNDMESSAGE_H_ */
