/*
 * Message.cpp
 *
 */

#include "Message.hpp"

namespace game_space {

Message::Message(MESSAGETYPE type) {
	_messageType = type;
}

Message::~Message() {
}

} /* namespace game_space */
