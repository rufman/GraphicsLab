/**
 * definitions.hpp
 * Definitions used through all of the game.
 */

#ifndef GAME_DEFS_HPP
#define GAME_DEFS_HPP

//############
// Coding definitions
//############

// NULL pointer
#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// Namespace definitions
#define GAME_NAMESPACE_BEGIN \
namespace game_space {

#define GAME_NAMESPACE_END \
}

#define GAME_NAMESPACE_QUALIFIER game_space::
#define GAME_NAMESPACE_USE using namespace game_space;

//#######################
// Application and window definitions
//#######################

// Default window title
#define WINDOW_TITLE "SillyTanks v0.1"

//#################
// Game Physics definitions
//#################

// Gravitational acceleration constant in m/s2
#define GRAVITATIONAL_ACCELERATION (-9.81)

//#################
// Tank definitions
//#################
#define SMALLTANK_CHASSIS_MODEL "resources/GFX/models/smalltank_chassis.ply"
#define TURRET_MODEL "resources/GFX/models/turret.ply"
#define SMALLTANK_SPEED 8

//#################
// Tower definitions
//#################
#define SMALLTOWER_TOWERBASE_MODEL "resource/GFX/models/"

//#################
//Tank textures definitions
//#################
#define SMALLTANK_TEXTURE1 "resources/GFX/textures/black.tga"
#define SMALLTANK_TEXTURE2 "resources/GFX/textures/blue.tga"
#define SMALLTANK_TEXTURE3 "resources/GFX/textures/cow.tga"

//#################
// Object map definitions
//#################
#define PINETREE_MODEL "resources/GFX/models/pinetree.ply"
#define PINETREE_MAPNR 0

#define PALMTREE_MODEL "resources/GFX/models/palmtree.ply"
#define PALMTREE_MAPNR 20

#define ROUNDTREE_MODEL "resources/GFX/models/roundtree.ply"
#define ROUNDTREE_MAPNR 40

//###########
// Particle engine
//###########
#define EXPLOSION_TEXTURE "resources/GFX/textures/explosion.tga"
#define SMOKE_TEXTURE "resources/GFX/textures/smoke.tga"

#define TOON_VERTEX_SHADER "ressources/shaders/toonvshader.vert"
#define TOON_FRAGMENT_SHADER "ressources/shaders/toonfshader.frag"
#endif // GAME_DEFS_HPP
