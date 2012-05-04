/**
 * tgaTexture.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   30.03.2012
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_TGATEXTURE_HPP
#define GRAPHICSLAB_TGATEXTURE_HPP


// Base class include
#include "Texture.hpp"

// Includes
#include <string>

GAME_NAMESPACE_BEGIN

// Forward declarations
class TGAImage;

/** TGA Texture */
class TGATexture : public Texture
{
public:
    /** 
	 * Constructor
	 *
	 * @param[in]	file	TGA image file name
	 */
    TGATexture( const std::string &file );
	
	/** Destructor */
	~TGATexture();

	// Get
	uint getWidth() const;
	uint getHeight() const;
	const uchar *getData() const;
	
private:
	TGAImage *_image;
	
}; // class TGATexture


GAME_NAMESPACE_END


#endif // GRAPHICSLAB_TGATEXTURE_HPP
