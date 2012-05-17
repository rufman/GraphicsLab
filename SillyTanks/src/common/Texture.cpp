/**
 * texture.cpp
 *
 */

// Class declaration include
#include "Texture.hpp"


namespace game_space {


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


}
