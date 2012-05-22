/**
 * gameSaveFunctions.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_GAMESAVEFUNCTIONS_HPP
#define GRAPHICSLAB_GAMESAVEFUNCTIONS_HPP

//Includes
#include "defs.hpp"
#include "scene.hpp"
#include "map.hpp"
#include "sun.hpp"
#include "player.hpp"
#include "utils.hpp"
#include "npcManager.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN

// Forward declarations
class Player;
class Sun;
class LoadingScreen;
class NPCManager;


class GameSaveFunctions {

public:

	GameSaveFunctions(Scene &scene, LoadingScreen* loadingScreen, Map* map, Sun* sun, Player* player, NPCManager* npcManager);

    ~GameSaveFunctions();

    void loadIntroMap();

    void loadGame(int gameSave);

    void saveGame(int gameSave);
    
private:
    static const int GAME_SAVE_FILE_LENGTH = 15;

    Scene& _scene;
    LoadingScreen* _loadingScreen;
    Map* _map;
    Sun* _sun;
    Player* _player;
    NPCManager* _npcManager;

};

GRAPHICSLAB_NAMESPACE_END

#endif  //  GRAPHICSLAB_GAMESAVEFUNCTIONS_HPP





