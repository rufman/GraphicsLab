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

	_isActive = false;

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

	setListenerValues(ListenerPos[0],ListenerPos[0],ListenerPos[2]);

}

void SoundEngine::setActive(bool value){
	_isActive = value;
}

void SoundEngine::setListenerValues(float x, float y,float z) {
	float arr[3] = {x,y,z};
	alListenerfv(AL_POSITION, arr);
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

#ifdef __APPLE__
	alutLoadWAVFile((ALbyte*) "resources/sounds/raygun.wav", &format, &data,
			&size, &freq);
#else
	alutLoadWAVFile((ALbyte*)"resources/sounds/raygun.wav", &format, &data, &size, &freq, &loop);
#endif
	alBufferData(Buffers[RAYGUN], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);


#ifdef __APPLE__
	alutLoadWAVFile((ALbyte*) "resources/sounds/enginerun.wav", &format, &data,
			&size, &freq);
#else
	alutLoadWAVFile((ALbyte*)"resources/sounds/enginerun.wav", &format, &data, &size, &freq, &loop);
#endif
	alBufferData(Buffers[ENGINE], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);


#ifdef __APPLE__
	alutLoadWAVFile((ALbyte*) "resources/sounds/enginerun.wav", &format, &data,
			&size, &freq);
#else
	alutLoadWAVFile((ALbyte*)"resources/sounds/enginerun.wav", &format, &data, &size, &freq, &loop);
#endif
	alBufferData(Buffers[ENGINE2], format, data, size, freq);
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



	alSourcei(Sources[RAYGUN], AL_BUFFER, Buffers[RAYGUN]);
	alSourcef(Sources[RAYGUN], AL_PITCH, 1.0f);
	alSourcef(Sources[RAYGUN], AL_GAIN, 1.0f);
	alSourcefv(Sources[RAYGUN], AL_POSITION, SourcesPos[RAYGUN]);
	alSourcefv(Sources[RAYGUN], AL_VELOCITY, SourcesVel[RAYGUN]);
	alSourcei(Sources[RAYGUN], AL_LOOPING, AL_FALSE);

	alSourcei(Sources[ENGINE], AL_BUFFER, Buffers[ENGINE]);
	alSourcef(Sources[ENGINE], AL_PITCH, 1.0f);
	alSourcef(Sources[ENGINE], AL_GAIN, 1.0f);
	alSourcefv(Sources[ENGINE], AL_POSITION, SourcesPos[ENGINE]);
	alSourcefv(Sources[ENGINE], AL_VELOCITY, SourcesVel[ENGINE]);
	alSourcei(Sources[ENGINE], AL_LOOPING, AL_FALSE);

	alSourcei(Sources[ENGINE2], AL_BUFFER, Buffers[ENGINE2]);
	alSourcef(Sources[ENGINE2], AL_PITCH, 1.0f);
	alSourcef(Sources[ENGINE2], AL_GAIN, 1.0f);
	alSourcefv(Sources[ENGINE2], AL_POSITION, SourcesPos[ENGINE2]);
	alSourcefv(Sources[ENGINE2], AL_VELOCITY, SourcesVel[ENGINE2]);
	alSourcei(Sources[ENGINE2], AL_LOOPING, AL_FALSE);
	float newVolume = 0.8f;
	alSourcef(Sources[ENGINE2], AL_GAIN, newVolume);

	// Do another error check and return.

	if (alGetError() != AL_NO_ERROR) {
		return AL_FALSE;
	}

	return AL_TRUE;
}

void SoundEngine::playGunSound() {

	if(_isActive){
		alSourcefv(Sources[GUN], AL_POSITION, SourcesPos[EXPLOSION]); // reset position of sound
		alSourcePlay(Sources[GUN]);
	}


}

void SoundEngine::playGunSoundAt(float x, float y, float z){

	if(_isActive){
		ALfloat arr[3] = {x, y, z};
		alSourcefv(Sources[GUN], AL_POSITION, arr);
		alSourcePlay(Sources[GUN]);
	}

}

void SoundEngine::playExplosionSound() {

	if(_isActive){
		alSourcefv(Sources[EXPLOSION], AL_POSITION, SourcesPos[EXPLOSION]); // reset position of sound
		alSourcePlay(Sources[EXPLOSION]);
	}
}

void SoundEngine::playExplosionSoundAt(float x, float y, float z){

	if(_isActive){
		ALfloat arr[3] = {x, y, z};
		alSourcefv(Sources[EXPLOSION], AL_POSITION, arr);
		alSourcePlay(Sources[EXPLOSION]);
	}
}

void SoundEngine::playMuzzleSound() {

	if(_isActive){
		alSourcefv(Sources[MUZZLE], AL_POSITION, SourcesPos[EXPLOSION]); //reset position of sound
		alSourcePlay(Sources[MUZZLE]);
	}
}

void SoundEngine::playMuzzleSoundAt(float x, float y, float z){

	if(_isActive){
		ALfloat arr[3] = {x, y, z};
		alSourcefv(Sources[MUZZLE], AL_POSITION, arr);
		alSourcePlay(Sources[MUZZLE]);
	}
}

void SoundEngine::playRayGunSoundAt(float x, float y, float z){

	if(_isActive){
		ALfloat arr[3] = {x, y, z};
		alSourcefv(Sources[RAYGUN], AL_POSITION, arr);
		alSourcePlay(Sources[RAYGUN]);
	}
}

void SoundEngine::playEngineSoundAt(float x, float y, float z){

	if(_isActive){
		ALfloat arr[3] = {x, y, z};
		alSourcefv(Sources[ENGINE], AL_POSITION, arr);
		alSourcePlay(Sources[ENGINE]);
	}
}

void SoundEngine::playEngineSound() {

	if(_isActive){
		alSourcefv(Sources[ENGINE], AL_POSITION, SourcesPos[ENGINE]); //reset position of sound
		alSourcePlay(Sources[ENGINE]);
	}
}

void SoundEngine::playOtherEngineSoundAt(float x, float y, float z){

	if(_isActive){
		ALfloat arr[3] = {x, y, z};
		alSourcefv(Sources[ENGINE2], AL_POSITION, arr);
		alSourcePlay(Sources[ENGINE2]);
	}
}

bool SoundEngine::isEngineSoundFinished(){
	ALint value;
	alGetSourcei(Sources[ENGINE], AL_SOURCE_STATE, &value);

	if(value == AL_STOPPED){
		return true;
	}else{
		return false;
	}
}


void SoundEngine::setEngineSoundPosition(float x, float y, float z){
	ALfloat arr[3] = {x, y, z};
	alSourcefv(Sources[ENGINE], AL_POSITION, arr);
}

void SoundEngine::changeVolumeMuzzle(float volume){
	if(volume <= 0){
		volume =0;
	}

	alSourcef(Sources[MUZZLE], AL_GAIN, volume);
}

void SoundEngine::changeVolumeExplosion(float volume){
	if(volume <= 0){
		volume =0;
	}

	alSourcef(Sources[EXPLOSION], AL_GAIN, volume);
}

void SoundEngine::changeVolumeEngine(float volume){
	if(volume <= 0){
		volume =0;
	}

	alSourcef(Sources[ENGINE], AL_GAIN, volume);
}



