/**
 * npcManager.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */


// Class declaration include
#include "npcManager.hpp"

// Includes
#include "scene.hpp"
#include "map.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN

NPCList NPCManager::_npcs;

NPCManager::NPCManager(Scene &scene, Map* map, Player* player) :
	_scene(scene),
	_map(map),
	_player(player) {
	_npcs = NPCList();
	NPC::init();
}


NPCManager::~NPCManager() {
}


void NPCManager::init() {
	NPC* bob = new NPC(_scene, _map, _player);
	bob->setSpawnPosition(880.0, 760.0);
	bob->setSpawnDirection(-45.0);
	bob->setRestlessness(2.0);
	bob->setLocomotionMode(true);
	bob->setTShirtColor(1.0, 0.0, 0.0);
	_npcs.push_back(bob);
	bob->assignID(_npcs.size() + 3);

	NPC* sam = new NPC(_scene, _map, _player);
	sam->setSpawnPosition(880.0, 762.0);
	sam->setSpawnDirection(-45.0);
	sam->setRestlessness(2.0);
	sam->setLocomotionMode(true);
	sam->setTShirtColor(1.0, 1.0, 0.0);
	_npcs.push_back(sam);
	sam->assignID(_npcs.size() + 3);

	NPC* bill = new NPC(_scene, _map, _player);
	bill->setSpawnPosition(882.0, 760.0);
	bill->setSpawnDirection(-45.0);
	bill->setRestlessness(2.0);
	bill->setLocomotionMode(true);
	bill->setTShirtColor(0.0, 1.0, 0.0);
	_npcs.push_back(bill);
	bill->assignID(_npcs.size() + 3);

	NPC* hal = new NPC(_scene, _map, _player);
	hal->setSpawnPosition(882.0, 762.0);
	hal->setSpawnDirection(-45.0);
	hal->setRestlessness(2.0);
	hal->setLocomotionMode(true);
	hal->setTShirtColor(0.0, 0.0, 1.0);
	_npcs.push_back(hal);
	hal->assignID(_npcs.size() + 3);
}


void NPCManager::spawnAll() {
	for(NPCList::iterator i = _npcs.begin(); i != _npcs.end(); ++i) {
		(*i)->spawn();
	}
}


void NPCManager::removeAll() {
	for(NPCList::iterator i = _npcs.begin(); i != _npcs.end(); ++i) {
		(*i)->remove();
	}
}


void NPCManager::update(float duration) {
	for(NPCList::iterator i = _npcs.begin(); i != _npcs.end(); ++i) {
		(*i)->update(duration);
	}
}


GRAPHICSLAB_NAMESPACE_END


