/**
 * gl_includes.hpp
 * GL includes are there to solve any problems with includes for each Operating System
 */

#ifndef GRAPHICSLAB_GL_INCLUDES_HPP
#define GRAPHICSLAB_GL_INCLUDES_HPP

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#ifdef WIN32

// Win32

#include <GL/glew.h>

#include <windows.h>
#include <glut.h>

#else
// MacOS X
#ifdef __APPLE__

#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#else
// UNIX, Linux

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#endif

#endif

#endif // GRAPHICSLAB_GL_INCLUDES_HPP
