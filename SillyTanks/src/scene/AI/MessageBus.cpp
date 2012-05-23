/*
 * MessageBus.cpp
 */

#include "MessageBus.hpp"
#include "Message.hpp"

namespace game_space {

MessageBus::MessageBus() {
}

MessageBus::~MessageBus() {
	//delete the message bus
	for (std::vector<std::vector<Message*>*>::iterator subBusIter = messageBus.begin(); subBusIter != messageBus.end();) {
		std::vector<Message*>* subBus = *subBusIter;
		for (std::vector<Message*>::iterator messageIter = subBus->begin(); messageIter != subBus->end();) {
			Message* message = *messageIter;
			messageIter = subBus->erase(messageIter);
			delete message;
		}
		subBusIter = messageBus.erase(subBusIter);
		delete subBus;
	}
}

std::vector<Message*>* MessageBus::addNewClient() {
	messageBus.push_back(new std::vector<Message*>);
	//return the messagebus client id
	return messageBus.at(messageBus.size() - 1);
}

std::vector<Message*>* MessageBus::getSubbusOfClient(int clientID) {
	return messageBus.at(clientID);
}

void MessageBus::sendMessageTo(Message message, int receiverID) {
	messageBus.at(receiverID)->push_back(&message);
}

}
