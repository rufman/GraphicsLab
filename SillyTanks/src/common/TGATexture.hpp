/**
 * tgaTexture.hpp
*
 */

#ifndef GRAPHICSLAB_TGATEXTURE_HPP
#define GRAPHICSLAB_TGATEXTURE_HPP


// Base class include
#include "Texture.hpp"

// Includes
#include <string>

namespace game_space {

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
	GLuint getID();
	
private:
	TGAImage *_image;
	
}; // class TGATexture


}


#endif // GRAPHICSLAB_TGATEXTURE_HPP
