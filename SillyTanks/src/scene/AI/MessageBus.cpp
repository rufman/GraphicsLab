/*
 * MessageBus.cpp
 */

#include "MessageBus.hpp"

namespace game_space {

MessageBus::MessageBus() {

}

MessageBus::~MessageBus() {

}

std::vector<Message*>* MessageBus::addNewClient()
{
	messageBus.push_back(new std::vector<Message*>);
	//return the messagebus client id
	return messageBus.at(messageBus.size()-1);
}

std::vector<Message*>* MessageBus::getSubbusOfClient(int clientID)
{
	return messageBus.at(clientID);
}

void MessageBus::sendMessageTo(Message message,int receiverID)
{
	messageBus.at(receiverID)->push_back(&message);
}



}
