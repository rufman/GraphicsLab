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

#define MOUSE_SENSITIVITY 0.05

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
#define SMALLTANK_VISION_DISTANCE 120
#define SMALLTANK_ESPACE_DISTANCE 80
#define SMALLTANK_RELOADING_TIME 3

#define SHOOTING_DISTANCE 30

//#################
// Tower definitions
//#################
#define SMALLTOWER_TOWERBASE_MODEL "resources/GFX/models/smalltower_towerbase.ply"
#define SMALLTOWER_VISION_DISTANCE 200
#define SMALLTOWER_RELOADING_TIME 3

//#################
//Tank textures definitions
//#################
#define SMALLTANK_TEXTURE1 "resources/GFX/textures/black.tga"
#define SMALLTANK_TEXTURE2 "resources/GFX/textures/blue.tga"
#define SMALLTANK_TEXTURE3 "resources/GFX/textures/brown.tga"
#define SMALLTANK_TEXTURE4 "resources/GFX/textures/bullseye.tga"
#define SMALLTANK_TEXTURE5 "resources/GFX/textures/cow.tga"
#define SMALLTANK_TEXTURE6 "resources/GFX/textures/darkgray.tga"
#define SMALLTANK_TEXTURE7 "resources/GFX/textures/fire.tga"
#define SMALLTANK_TEXTURE8 "resources/GFX/textures/grayblue.tga"
#define SMALLTANK_TEXTURE9 "resources/GFX/textures/green.tga"
#define SMALLTANK_TEXTURE10 "resources/GFX/textures/king.tga"
#define SMALLTANK_TEXTURE11 "resources/GFX/textures/nuclear.tga"
#define SMALLTANK_TEXTURE12 "resources/GFX/textures/pink.tga"
#define SMALLTANK_TEXTURE13 "resources/GFX/textures/red.tga"
#define SMALLTANK_TEXTURE14 "resources/GFX/textures/sand.tga"
#define SMALLTANK_TEXTURE15 "resources/GFX/textures/slate.tga"
#define SMALLTANK_TEXTURE16 "resources/GFX/textures/snow.tga"


//#######################
// Robot definitions
//#######################
#define ROBOT_TIMETOLIVE 8

//#################
// Object map definitions
//#################
#define PINETREE_MODEL "resources/GFX/models/pinetree.ply"
#define PINETREE_TEXTURE "resources/GFX/textures/targets/trees/pinetree.tga"
#define PINETREE_MAPNR 0

#define PALMTREE_MODEL "resources/GFX/models/palmtree.ply"
#define PALMTREE_MAPNR 20

#define ROUNDTREE_MODEL "resources/GFX/models/roundtree.ply"
#define ROUNDTREE_TEXTURE "resources/GFX/textures/targets/trees/roundtree.tga"
#define ROUNDTREE_MAPNR 40

//###########
// Particle engine
//###########
#define EXPLOSION_TEXTURE "resources/GFX/textures/explosion.tga"
#define SMOKE_TEXTURE "resources/GFX/textures/smoke.tga"

#define TOON_VERTEX_SHADER "resources/shaders/toonvshader_notex.vert"
#define TOON_FRAGMENT_SHADER "resources/shaders/toonfshader_notex.frag"
#endif // GAME_DEFS_HPP
