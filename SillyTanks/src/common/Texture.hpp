/**
 * texture.hpp
*
 */

#ifndef GRAPHICSLAB_TEXTURE_HPP
#define GRAPHICSLAB_TEXTURE_HPP


// Includes
#include "Definitions.hpp"
#include "Types.hpp"
#include "GLIncludes.hpp"


namespace game_space {


/** Texture */
class Texture
{
public:
    /** Constructor */
    Texture( GLenum target = GL_TEXTURE_RECTANGLE_ARB );
    
    /** Destructor */
    virtual ~Texture();
	    
    /** 
     * Set the texture as current active texture.
     * One has to activate this to true before using the texture,
     * and then deactivate it after usage.
     *
     * @param[in]   on      Whether to turn on or off
     */
    virtual void setActive( bool on = true ) const;
		
protected:
    /** Texture ID */
    GLuint _id;
    
	/** Texture target */
	GLenum _target;

}; // class Texture


}


#endif // GRAPHICSLAB_TEXTURE_HPP
