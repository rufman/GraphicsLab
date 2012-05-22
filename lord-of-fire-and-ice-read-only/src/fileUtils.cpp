/**
 * fileUtils.cpp
 * 
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "fileUtils.hpp"

// Includes

GRAPHICSLAB_NAMESPACE_BEGIN

uint8_t FileUtils::readByte( FILE *f )
{
    uint8_t value;
    fread( &value, 1, 1, f );
    
    return value;
}

uint16_t FileUtils::readShort( FILE *f )
{
    short a, b;
    a = readByte( f );
    b = readByte( f );
    
    return ( a + ( b<<8 ) );
}

GRAPHICSLAB_NAMESPACE_END


