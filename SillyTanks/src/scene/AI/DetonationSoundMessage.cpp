/*
 * DetonationSoundMessage.cpp
* This message is sent to an AI if there was a detonation somewhere on the map that is hearable to other tanks.
 */

#include "DetonationSoundMessage.hpp"

namespace game_space {

DetonationSoundMessage::DetonationSoundMessage(Point detonationPoint,float detonationStrength) :
		Message(Message::DETONATION_SOUND), _detonationPoint(detonationPoint), _detonationStrength(detonationStrength) {
}

DetonationSoundMessage::~DetonationSoundMessage() {

}

} /* namespace game_space */
