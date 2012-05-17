/*
 * DetonationSoundMessage.h
 *
 *  Created on: May 17, 2012
 *      Author: leviathan
 */

#ifndef DETONATIONSOUNDMESSAGE_H_
#define DETONATIONSOUNDMESSAGE_H_

#include "Message.hpp"

namespace game_space {

class DetonationSoundMessage: public game_space::Message {
public:
	DetonationSoundMessage(Point _detonationPoint);
	virtual ~DetonationSoundMessage();
};

} /* namespace game_space */
#endif /* DETONATIONSOUNDMESSAGE_H_ */
