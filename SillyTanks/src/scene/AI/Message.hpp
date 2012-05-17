/*
 * Message.h
 *
 *  Created on: May 17, 2012
 *      Author: leviathan
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

namespace game_space {

class Message {
public:
	enum MESSAGETYPE{
		DETONATION_SOUND,
		ATTACKED_BY
	}_messageType;
public:
	Message(MESSAGETYPE type);
	virtual ~Message();
};

} /* namespace game_space */
#endif /* MESSAGE_H_ */
