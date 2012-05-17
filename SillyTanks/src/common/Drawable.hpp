/**
 * drawable.hpp
* This is the base class for every drawable object in this game.
 */

#ifndef GRAPHICSLAB_DRAWABLE_HPP
#define GRAPHICSLAB_DRAWABLE_HPP


// Global definitions include
#include "Definitions.hpp"

// Includes
#include "Types.hpp"


namespace game_space {


// Forward declarations
class Scene;

/** Abstract base class to provide interface for a drawable object in the scene */
class Drawable
{
public:
    /** 
	 * Constructor
	 *
	 * @parameter[in, out]	scene	Owning scene
	 */
    Drawable( Scene &scene ) :
        _scene( scene )
    {}
    
    /** Destructor */
    virtual ~Drawable() {}
        
	/**
	 * Set rendering parameters.
	 *
	 * @param[in]	renderingParameters		Rendering parameters
	 */
    void setRenderingParameters( const RenderingParameters &renderingParameters ) { _renderingParameters = renderingParameters; }
    
    /** Draw the object to the screen. */
    virtual void draw() const = 0;

protected:
    /** Owning scene */
    Scene &_scene;
    
    /** Rendering parameters */
    RenderingParameters _renderingParameters;
    
}; // class Drawable


}


#endif // GRAPHICSLAB_DRAWABLE_HPP
