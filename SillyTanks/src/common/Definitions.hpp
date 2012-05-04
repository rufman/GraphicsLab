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
// Game Graphics definitions
//#################
#define SMALLTANK_CHASSIS_MODEL "resources/GFX/models/smalltank_chassis.ply"
#define SMALLTANK_TURRET_MODEL "resources/GFX/models/smalltank_turret.ply"
#define SMALLTANK_SPEED 8
#define SMALLTANK_BOOST 15

//###########
// Particle engine
//###########
#define SPARKLE_TEXTURE "resources/GFX/textures/sparkle.tga"
#define SMOKE_TEXTURE "resources/GFX/textures/smoke.tga"
#endif // GAME_DEFS_HPP
