/*
 * SoundEngine.cpp
 *
 *  Created on: 17.05.2012
 *      Author: gmazlami
 */

#include "SoundEngine.hpp"
#include <stdlib.h>
#include <math.h>
#include <iostream>

SoundEngine::SoundEngine() {

	ListenerPos[0] = 0.0;
	ListenerPos[1] = 0.0;
	ListenerPos[2] = 0.0;

	ListenerVel[0] = 0.0;
	ListenerVel[1] = 0.0;
	ListenerVel[2] = 0.0;

	ListenerOri[0] = 0.0;
	ListenerOri[1] = 0.0;
	ListenerOri[2] = -1.0;
	ListenerOri[3] = 0.0;
	ListenerOri[4] = 1.0;
	ListenerOri[5] = 0.0;

	// Initialize OpenAL and clear the error bit.
	alutInit(NULL, 0);
	alGetError();

	// Load the wav data.

	if (LoadALData() == AL_FALSE) {
		std::cerr << "ERROR during loading of AL data" << std::endl;
		//exit(1);
	}

	SetListenerValues();

}

void SoundEngine::SetListenerValues() {
	alListenerfv(AL_POSITION, ListenerPos);
	alListenerfv(AL_VELOCITY, ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}

void SoundEngine::KillALData() {
	alDeleteBuffers(NUM_BUFFERS, Buffers);
	alDeleteSources(NUM_SOURCES, Sources);
	alutExit();
}

ALboolean SoundEngine::LoadALData() {
	// Variables to load into.

	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;

	// Load wav data into buffers.

	alGenBuffers(NUM_BUFFERS, Buffers);

	if (alGetError() != AL_NO_ERROR) {
		return AL_FALSE;
	}

//This whole mess needs to be done, because Mac OSX ommited the &loop parameter...
#ifdef __APPLE__
	alutLoadWAVFile((ALbyte*) "resources/sounds/bomb2.wav", &format, &data,
			&size, &freq);
#else
	alutLoadWAVFile((ALbyte*)"resources/sounds/bomb2.wav", &format, &data, &size, &freq, &loop);
#endif

	alBufferData(Buffers[EXPLOSION], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

#ifdef __APPLE__
	alutLoadWAVFile((ALbyte*) "resources/sounds/Gun1.wav", &format, &data,
			&size, &freq);
#else
	alutLoadWAVFile((ALbyte*)"resources/sounds/Gun1.wav", &format, &data, &size, &freq, &loop);
#endif

	alBufferData(Buffers[GUN], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

#ifdef __APPLE__
	alutLoadWAVFile((ALbyte*) "resources/sounds/muzzleshot.wav", &format, &data,
			&size, &freq);
#else
	alutLoadWAVFile((ALbyte*)"resources/sounds/muzzleshot.wav", &format, &data, &size, &freq, &loop);
#endif
	alBufferData(Buffers[MUZZLE], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	// Bind buffers into audio sources.

	alGenSources(NUM_SOURCES, Sources);

	if (alGetError() != AL_NO_ERROR) {
		return AL_FALSE;
	}

	alSourcei(Sources[EXPLOSION], AL_BUFFER, Buffers[EXPLOSION]);
	alSourcef(Sources[EXPLOSION], AL_PITCH, 1.0f);
	alSourcef(Sources[EXPLOSION], AL_GAIN, 1.0f);
	alSourcefv(Sources[EXPLOSION], AL_POSITION, SourcesPos[EXPLOSION]);
	alSourcefv(Sources[EXPLOSION], AL_VELOCITY, SourcesVel[EXPLOSION]);
	alSourcei(Sources[EXPLOSION], AL_LOOPING, AL_FALSE);

	alSourcei(Sources[GUN], AL_BUFFER, Buffers[GUN]);
	alSourcef(Sources[GUN], AL_PITCH, 1.0f);
	alSourcef(Sources[GUN], AL_GAIN, 1.0f);
	alSourcefv(Sources[GUN], AL_POSITION, SourcesPos[GUN]);
	alSourcefv(Sources[GUN], AL_VELOCITY, SourcesVel[GUN]);
	alSourcei(Sources[GUN], AL_LOOPING, AL_FALSE);

	alSourcei(Sources[MUZZLE], AL_BUFFER, Buffers[MUZZLE]);
	alSourcef(Sources[MUZZLE], AL_PITCH, 1.0f);
	alSourcef(Sources[MUZZLE], AL_GAIN, 1.0f);
	alSourcefv(Sources[MUZZLE], AL_POSITION, SourcesPos[MUZZLE]);
	alSourcefv(Sources[MUZZLE], AL_VELOCITY, SourcesVel[MUZZLE]);
	alSourcei(Sources[MUZZLE], AL_LOOPING, AL_FALSE);

	// Do another error check and return.

	if (alGetError() != AL_NO_ERROR) {
		return AL_FALSE;
	}

	return AL_TRUE;
}

void SoundEngine::playGunSound() {

	alSourcefv(Sources[GUN], AL_POSITION, SourcesPos[EXPLOSION]); // reset position of sound
	alSourcePlay(Sources[GUN]);

}

void SoundEngine::playGunSoundAt(float x, float y, float z){


	ALfloat arr[3] = {x, y, z};
	alSourcefv(Sources[GUN], AL_POSITION, arr);
	alSourcePlay(Sources[GUN]);
}

void SoundEngine::playExplosionSound() {

	alSourcefv(Sources[EXPLOSION], AL_POSITION, SourcesPos[EXPLOSION]); // reset position of sound
	alSourcePlay(Sources[EXPLOSION]);

}

void SoundEngine::playExplosionSoundAt(float x, float y, float z){

	ALfloat arr[3] = {x, y, z};
	alSourcefv(Sources[EXPLOSION], AL_POSITION, arr);
	alSourcePlay(Sources[EXPLOSION]);

}

void SoundEngine::playMuzzleSound() {

	alSourcefv(Sources[MUZZLE], AL_POSITION, SourcesPos[EXPLOSION]); //reset position of sound
	alSourcePlay(Sources[MUZZLE]);

}

void SoundEngine::playMuzzleSoundAt(float x, float y, float z){

	ALfloat arr[3] = {x, y, z};
	alSourcefv(Sources[MUZZLE], AL_POSITION, arr);
	alSourcePlay(Sources[MUZZLE]);

}



