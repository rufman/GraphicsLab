/**
 * texture.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_TEXTURE_HPP
#define GRAPHICSLAB_TEXTURE_HPP

// Includes
#include "defs.hpp"
#include "types.hpp"
#include "gl_includes.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN


/** Texture to load from an image */
class Texture
{
public:
    /** Constructor */
    Texture( GLenum target = GL_TEXTURE_RECTANGLE_ARB );
    
    /** Destructor */
    virtual ~Texture();

    /** Reset data */
    virtual void reset();
    
    /** Initialize texture data */
    virtual void init() = 0;

	/** 
	 * Get image data for the texture.
	 *
	 * @return	Image data array in RGBA format with uchar for each component
	 */
	virtual uchar *getImage() const;

    
    /** 
     * Set the texture as current active texture.
     * One has to activate this to true before using the texture,
     * and then deactivate it after usage.
     *
     * @param[in]   on      Whether to turn on or off
     */
    virtual void setActive( bool on = true ) const;
    
    float getWidth() const { return _width; }
    float getHeight() const { return _height; }
    GLenum getTarget() const { return _target; }
    

protected:
    /** Texture ID */
    GLuint _id;
    
	/** Texture target */
	GLenum _target;
    
    /** Texture size */
    float _width, _height;

}; // class Texture


GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_TEXTURE_HPP


