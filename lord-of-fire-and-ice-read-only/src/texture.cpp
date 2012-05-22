/**
 * texture.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "texture.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN

Texture::Texture( GLenum target ) :
	_target( target ),
    _width( 1.0 ), _height( 1.0 )
{    
    glGenTextures( 1, &_id );
}

Texture::~Texture()
{
    glDeleteTextures( 1, &_id );
}

void Texture::reset()
{
}

uchar *Texture::getImage() const
{    
	glBindTexture( _target, _id );
	GLint width, height;
	glGetTexLevelParameteriv( _target, 0, GL_TEXTURE_WIDTH, &width );
	glGetTexLevelParameteriv( _target, 0, GL_TEXTURE_HEIGHT, &height);
	uchar *data = new uchar[height*width*4];
	glGetTexImage( _target, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    
	return data;
}

void Texture::setActive( bool on ) const
{
    if ( on )
    {
        glEnable( _target );
        glBindTexture( _target, _id );
		glTexParameteri( _target, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( _target, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( _target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( _target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    else
    {
        glDisable( _target );
    }
    
}


GRAPHICSLAB_NAMESPACE_END

