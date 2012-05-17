/*
 * MessageBus.cpp
 */

#include "MessageBus.hpp"

namespace game_space {

MessageBus::MessageBus() {

}

MessageBus::~MessageBus() {

}

int MessageBus::addNewClient()
{
	messageBus.push_back(new std::vector<Message>);
	//return the messagebus client id
	return messageBus.size()-1;
}

std::vector<Message>* MessageBus::getMessagesForClient(int clientID)
{
	return messageBus.at(clientID);
}



}
