/**
 * reflectionTexture.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */
#ifndef GRAPHICSLAB_REFLECTIONTEXTURE_HPP
#define GRAPHICSLAB_REFLECTIONTEXTURE_HPP

// Base class include
#include "texture.hpp"

// Includes
#include "defs.hpp"
#include "types.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN


/** Texture to load from current drawn screen */
class ReflectionTexture : public Texture
{
public:
	/** Constructor */
	ReflectionTexture();

    /** Initialize texture data */
    void init();

	/** Grab screen to use as texture */
	void grabScreen();

	/** 
	 * Get image data for the texture.
	 *
	 * @return	Image data array
	 */
	uchar *getImage() const;

	static int getViewportHeight();
	static int getViewportWidth();
	static float getViewportRatio();

	static float applyViewportRatioX(float& coord);
	static float applyViewportRatioY(float& coord);

	static void activateDrawing(bool on);
	static bool isDrawing();

private:
	static const int VIEWPORT_HEIGHT = 512;
	static bool _on;
	static int _viewportWidth;
	static float _viewportRatio;

};


GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_REFLECTIONTEXTURE_HPP


