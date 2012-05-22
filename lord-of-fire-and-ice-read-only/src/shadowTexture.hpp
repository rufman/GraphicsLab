/**
 * shadowTexture.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */
#ifndef GRAPHICSLAB_SHADOWTEXTURE_HPP
#define GRAPHICSLAB_SHADOWTEXTURE_HPP

// Base class include
#include "texture.hpp"

// Includes
#include "defs.hpp"
#include "types.hpp"
#include "camera3D.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN


/** Texture to load from current drawn screen */
class ShadowTexture : public Texture
{
public:
	/** Constructor */
	ShadowTexture();

	~ShadowTexture();

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

	void saveTexture();

	static void setCenter(vec3f playerPosition);

	static int getViewportHeight();
	static int getViewportWidth();

	static void activateDrawing(bool on);
	static bool isDrawing();

	static LookAt getCameraLookAt();
	static float getCameraFieldOfView();

	static int getCenterX();
	static int getCenterY();

	static float getShadowColor(float x, float y);

	static const int SHADOW_PIXELS_PER_SQUARE = 4;
	static const int SIZE_OF_SHADOWED_SURFACE = 128;
	static const float SHADOW_ALPHA = 0.35;
private:
	static int _centerX;
	static int _centerY;
	static bool _on;
	static int _viewportWidth;
	static int _viewportHeight;
	static float _cameraFieldOfView;
	static uchar* _textureData;

};


GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_SHADOWTEXTURE_HPP


