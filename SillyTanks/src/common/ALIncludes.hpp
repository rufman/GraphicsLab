/**
 * AL includes are there to solve any problems with includes for each Operating System
 */

#ifndef GRAPHICSLAB_AL_INCLUDES_HPP
#define GRAPHICSLAB_AL_INCLUDES_HPP

// MacOS X
#ifdef __APPLE__

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <ALUT/alut.h>

#else
// UNIX, Linux

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#endif

#endif
