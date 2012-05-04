/**
 * texture.cpp
 *
 * Author   :   Fatih Erol
 * Date     :   30.03.2012
 *
 * All rights reserved.
 */

// Class declaration include
#include "Texture.hpp"


GAME_NAMESPACE_BEGIN


Texture::Texture( GLenum target ) :
	_target( target )
{    
    glGenTextures( 1, &_id );
}

Texture::~Texture()
{
    glDeleteTextures( 1, &_id );
}

void Texture::setActive( bool on ) const
{
    if ( on )
    {
        glEnable( _target );
        glTexParameteri( _target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( _target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( _target, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri( _target, GL_TEXTURE_WRAP_T, GL_CLAMP );
        glBindTexture( _target, _id );
    }
    else
    {
        glDisable( _target );
    }
}


GAME_NAMESPACE_END
