/**
 * directionalLight.hpp
 *
 * Author   :   Fatih Erol
 * Date     :   19.03.2012
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_DIRECTIONALLIGHT_HPP
#define GRAPHICSLAB_DIRECTIONALLIGHT_HPP


// Base class include
#include "Light.hpp"


GAME_NAMESPACE_BEGIN


/** 
 * Class for a directional light, which is a light 
 * at the infinity with a direction, where the light rays
 * arrive in parallel.
 */
class DirectionalLight : public Light
{
public:
	/**
	 * Constructor.
	 * 
	 * @param[in]	scene	Owning scene
	 */
    DirectionalLight( Scene &scene );

	/** Destructor */
    virtual ~DirectionalLight();
    
    /**
     * Draw the object to the screen.
     */
    virtual void draw() const;

	// Set properties of light
    virtual void setPosition( const Point &position );
    virtual void setConstantAttenuation( const float constantAttenuation );
    virtual void setLinearAttenuation( const float linearAttenuation );
    virtual void setQuadraticAttenuation( const float quadraticAttenuation );
    
}; // class DirectionalLight


GAME_NAMESPACE_END


#endif // GRAPHICSLAB_DIRECTIONALLIGHT_HPP
