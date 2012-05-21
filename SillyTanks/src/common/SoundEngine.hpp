/*
 * SoundEngine.hpp
 *
 *  Created on: 17.05.2012
 *      Author: gmazlami
 */

#ifndef SOUNDENGINE_HPP_
#define SOUNDENGINE_HPP_

// Maximum data buffers we will need.
#define NUM_BUFFERS 3

// Maximum emissions we will need.
#define NUM_SOURCES 3

// These index the buffers and sources.
#define EXPLOSION 0
#define GUN   1
#define MUZZLE   2

#include <stdlib.h>
#include <math.h>
#include <iostream>
// common includes
#include "../common/ALIncludes.hpp"

class SoundEngine {

public:

	SoundEngine();

	void playGunSound();

	void playGunSoundAt(float x, float y, float z);

	void playExplosionSound();

	void playExplosionSoundAt(float x, float y, float z);

	void playMuzzleSound();

	void playMuzzleSoundAt(float x, float y, float z);




private:

	void SetListenerValues();

	ALboolean LoadALData();

	void KillALData();

	// Buffers hold sound data.
	ALuint Buffers[NUM_BUFFERS];

	// Sources are points of emitting sound.
	ALuint Sources[NUM_SOURCES];



	// Position of the source sounds.
	ALfloat SourcesPos[NUM_SOURCES][3];

	// Velocity of the source sounds.
	ALfloat SourcesVel[NUM_SOURCES][3];



	// Position of the listener.
	ALfloat ListenerPos[];

	// Velocity of the listener.
	ALfloat ListenerVel[] ;

	// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
	ALfloat ListenerOri[] ;

};

#endif /* SOUNDENGINE_HPP_ */
