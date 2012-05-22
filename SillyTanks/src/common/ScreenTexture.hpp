/**
 * screenTexture.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   12.04.2012
 *
 * All rights reserved....
 */

#ifndef GRAPHICSLAB_SCREENTEXTURE_HPP
#define GRAPHICSLAB_SCREENTEXTURE_HPP


// Base class include
#include "Texture.hpp"

namespace game_space {


/** Screen Texture */
class ScreenTexture : public Texture
{
public:
    /** Constructor */
    ScreenTexture();
	
	/** Destructor */
	~ScreenTexture();
	
	/**
	 * Capture rectangular region of current screen contents.
	 *
	 * @param[in]	x		x coordinate of the rectangular region - bottom left
	 * @param[in]	y		y coordinate of the rectangular region - bottom left
	 * @param[in]	width	width of the rectangular region
	 * @param[in]	height	height of the rectangular region
	 */
	void capture( uint x, uint y, uint width, uint height );
	
	// Get
	uint getWidth() const { return _width; }
	uint getHeight() const { return _height; }
	
private:
	uint _width, _height;
	
}; // class ScreenTexture

}

#endif // GRAPHICSLAB_SCREENTEXTURE_HPP
