/**
 * positionalLight.hpp
*
 */


#ifndef GRAPHICSLAB_POSITIONALLIGHT_HPP
#define GRAPHICSLAB_POSITIONALLIGHT_HPP

// Base class include
#include "Light.hpp"


GAME_NAMESPACE_BEGIN


/** Class for a positional point light source */
class PositionalLight : public Light
{
public:
	/**
	 * Constructor.
	 *
	 * @param[in]	scene	Owning scene
	 */
    PositionalLight( Scene &scene );

	/** Destructor */
    virtual ~PositionalLight();
    
    virtual void draw() const;

}; // class PositionalLight


GAME_NAMESPACE_END


#endif // GRAPHICSLAB_POSITIONALLIGHT_HPP
