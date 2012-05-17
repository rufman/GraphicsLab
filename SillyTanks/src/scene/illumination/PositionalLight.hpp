/**
 * positionalLight.hpp
*
 */


#ifndef GRAPHICSLAB_POSITIONALLIGHT_HPP
#define GRAPHICSLAB_POSITIONALLIGHT_HPP

// Base class include
#include "Light.hpp"


namespace game_space {


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


}


#endif // GRAPHICSLAB_POSITIONALLIGHT_HPP
