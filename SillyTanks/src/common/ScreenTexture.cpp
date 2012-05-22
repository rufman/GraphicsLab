/**
 * screenTexture.cpp
 * 
 * Author   :   Fatih Erol
 * Date     :   12.04.2012
 *
 * All rights reserved.
 */

// Class declaration include
#include "ScreenTexture.hpp"


namespace game_space {


ScreenTexture::ScreenTexture() :
	Texture(),
	_width( 0 ), _height( 0 )
{
}

ScreenTexture::~ScreenTexture()
{

}

void ScreenTexture::capture( uint x, uint y, uint width, uint height )
{	
	glTexParameteri( _target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( _target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( _target, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( _target, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glBindTexture( _target, _id );
	glCopyTexImage2D( _target, 0, GL_RGBA, x, y, width, height, 0 );
	_width = width; _height = height;
}

}
