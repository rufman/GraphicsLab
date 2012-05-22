/**
 * npcManager.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_NPCMANAGER_HPP
#define GRAPHICSLAB_NPCMANAGER_HPP

//Includes
#include "defs.hpp"
#include "scene.hpp"
#include "map.hpp"
#include "utils.hpp"
#include "npc.hpp"
#include "player.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN

// Forward declarations
class NPC;
class Player;
typedef std::vector <NPC*> NPCList;

class NPCManager {

public:

	NPCManager(Scene &scene, Map* map, Player* player);

    ~NPCManager();

    void init();
    
    void spawnAll();

    void removeAll();

    void update(float duration);

private:
    static const int NPC_ARRAY_LENGTH = 512;

    Scene& _scene;
    Map* _map;
    Player* _player;

    static NPCList _npcs;

};

GRAPHICSLAB_NAMESPACE_END

#endif  //  GRAPHICSLAB_NPCMANAGER_HPP





