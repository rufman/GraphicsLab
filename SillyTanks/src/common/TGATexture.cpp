/**
 * tgaTexture.cpp
 * 
 * Author   :   Fatih Erol
 * Date     :   30.03.2012
 *
 * All rights reserved.
 */

// Class declaration include
#include "TGATexture.hpp"

// Includes
#include "Exception.hpp"
#include "TGAImage.hpp"

#include <sstream>


GAME_NAMESPACE_BEGIN


TGATexture::TGATexture( const std::string &file ) :
	Texture(),
	_image( new TGAImage() )
{
	_image->load( file );
	
	glTexParameteri( _target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( _target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( _target, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( _target, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glBindTexture( _target, _id );
    glTexImage2D( _target, 0, GL_RGBA, _image->getWidth(), _image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _image->getData() );
}

TGATexture::~TGATexture()
{
	delete _image;
}

uint TGATexture::getWidth() const 
{
	return _image->getWidth();
}

uint TGATexture::getHeight() const
{
	return _image->getHeight();
}

const uchar *TGATexture::getData() const
{
	return _image->getData();
}

GAME_NAMESPACE_END
