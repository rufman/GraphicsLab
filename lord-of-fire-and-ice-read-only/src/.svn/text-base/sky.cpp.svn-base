/**
 * sky.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   16.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "sky.hpp"
#include "gl_includes.hpp"
#include "defs.hpp"
#include "map.hpp"
#include "utils.hpp"
#include <cmath>

#define yeeees true
#define ohnooo false

GRAPHICSLAB_NAMESPACE_BEGIN	

Sky::Sky ( Map* map ) { 
	_map = map ;
	_skyTexture = new TGATexture ( "data/textures/clouds.tga" ) ;
	_skyTexture -> init () ;
	_rotationAngle = 0.0;
}

Sky::~Sky () {
	delete _skyTexture ;
}

void Sky::update ( float duration ) {
	_rotationAngle += duration * 0.2;
	
	if ( _rotationAngle > 360 ) {
		_rotationAngle -= 360;
	}
	
}

void Sky::draw () { 

	glPushMatrix () ;
	
	_map->applyReflectionTransformation();

	glDisable ( GL_DEPTH ) ;
	glDisable ( GL_LIGHTING ) ;
	glDisable ( GL_FOG ) ;
	glEnable ( GL_BLEND ) ;
	glBlendFunc ( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA ) ;
		
		glColor3f ( 255 , 255 , 255 ) ;
		_skyTexture -> setActive ( true ) ;
		glTranslated ( _map -> getMapWidth () / 2 , -500.0 , _map -> getMapHeight () / 2 ) ;
		glScalef ( 1.0 , 0.2 , 1.0 ) ;
		glRotated ( 90 , 1 , 0 , 0 ) ;
		glRotated ( _rotationAngle , 0 , 1 , 0 ) ;
		Utils::drawTexturedSphere ( 5000 , 64 , 32 , 0 , 0 , 2048 , 1024 ) ;
		_skyTexture -> setActive ( false ) ;

	glDisable ( GL_BLEND ) ;
	glEnable ( GL_FOG ) ;
	glEnable ( GL_LIGHTING ) ;
	glEnable ( GL_DEPTH ) ;

	_map->removeReflectionTransformation();

	glPopMatrix () ;

}

GRAPHICSLAB_NAMESPACE_END
