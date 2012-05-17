/**
 * tgaImage.hpp
*
 */

#ifndef GRAPHICSLAB_TGAIMAGE_HPP
#define GRAPHICSLAB_TGAIMAGE_HPP


// Global definitions include
#include "Definitions.hpp"

// Includes
#include "Types.hpp"
#include <string>

namespace game_space {


/** TGA Image */
class TGAImage
{
public:
    /** Constructor */
    TGAImage();
    
    /** Destructor */
    ~TGAImage();
    
	/**
	 * Load TGA image from a file.
	 *
	 * @param[in]	file	File to load PLY data from
	 */
	void load( const std::string &file );
	
	// Get
	uint getWidth() const { return _width; }
	uint getHeight() const { return _height; }

	/**
	 * Get image data.
	 *
	 * @return		Image data
	 */
	const uchar *getData() const { return _data; }
	
private:
	void reset();
	
private:
	uint _width, _height;
	uchar *_data;
	
}; // class TGAImage


}


#endif // GRAPHICSLAB_TGAIMAGE_HPP
