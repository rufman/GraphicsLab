/**
 * tgaImage.cpp
*
 */

// Class declaration include
#include "TGAImage.hpp"

// common includes
#include "Exception.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdint.h>

#ifdef WIN32
#include <cstdint>
#endif


namespace game_space {


TGAImage::TGAImage() :
	_width( 0 ),
	_height( 0 ),
	_data( NULL )
{
}

TGAImage::~TGAImage()
{
	delete [] _data;
}

void TGAImage::reset()
{
	_width = _height = 0;	
	delete [] _data; _data = NULL;
}

void TGAImage::load( const std::string &file )
{
	reset();
	
	std::ifstream f( file.c_str(), std::ios::in | std::ios::binary );
	if ( !f.is_open() )
	{
		std::stringstream error; error << "Failed to open file " << file.c_str();
		throw Exception( error.str() );
	}

	uint8_t idLength; f.read( (char*)&idLength, 1 );
    uint8_t mapType; f.read( (char*)&mapType, 1 );
    uint8_t imageType; f.read( (char*)&imageType, 1 );
    uint16_t mapOrigin; f.read( (char*)&mapOrigin, 2 );
    uint16_t mapLength; f.read( (char*)&mapLength, 2 );
    uint8_t mapWidth; f.read( (char*)&mapWidth, 1 );
    uint16_t xOrigin; f.read( (char*)&xOrigin, 2 );
    uint16_t yOrigin; f.read( (char*)&yOrigin, 2 );
    uint16_t imageWidth; f.read( (char*)&imageWidth, 2 );
    uint16_t imageHeight; f.read( (char*)&imageHeight, 2 );
    uint8_t pixelDepth; f.read( (char*)&pixelDepth, 1 );
    uint8_t imageDesc; f.read( (char*)&imageDesc, 1 );

	if ( imageType != 2 )
	{
        std::stringstream error;
        error << "Only uncompressed RGB images are supported: " << imageType;
        throw Exception( error.str() );
	}
	
	
    int ordering = ( ( imageDesc&0xff0000 ) >> 4 );
	if ( ordering == 1 || ordering == 3 )
    {
        std::stringstream error;
        error << "Ordering of image is not supported";
        throw Exception( error.str() );
    }
    
    // Pull out all of the id data    
    char  szJunk[65535];
	f.read( szJunk, idLength );

	int depth  = pixelDepth >> 3;  
    if ( depth == 2 )
    {
        std::stringstream error;
        error << "Only 8, 24 or 32bit images supported";
        throw Exception( error.str() );
    }

    int size = imageWidth * imageHeight * 4;
    
    _width = imageWidth;
	_height = imageHeight;
	_data = new uchar[size];
	    
    uint8_t buffer[4];
    for ( uint i = 0; i < _height; i++ )
    {
        for ( uint j = 0; j < _width; j++ )
        {
			f.read( (char*)buffer, depth );
            switch ( depth )
            {
                case 1:
                    buffer[1] = buffer[2] = buffer[0];
                    buffer[3] = 255;
                    break;
                case 3:
                    buffer[3] = 255;
                    break;
            }
			
            uint index;
            if ( ordering == 0 )
            {
                index = ( _height - 1 - i ) * _width * 4 + j * 4;
            }
            else
            {
                index = i * _width * 4 + j * 4;
            }
            _data[index + 0] = buffer[2];
            _data[index + 1] = buffer[1];
            _data[index + 2] = buffer[0];
            _data[index + 3] = buffer[3];            
        }
    }
    
	f.close();
}


}
