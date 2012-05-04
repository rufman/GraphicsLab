/**
 * spotLight.hpp
 * 
 * Author   :   Fatih Erol
 * Date     :   19.03.2012
 */

#ifndef GRAPHICSLAB_SPOTLIGHT_HPP
#define GRAPHICSLAB_SPOTLIGHT_HPP


// Base class include
#include "Light.hpp"


GAME_NAMESPACE_BEGIN


/** Light to simulate a spot light */
class SpotLight : public Light
{
public:
    /**
     * Constructor.
     * 
     * @param[in]   scene   Owning scene
     */
    SpotLight( Scene &scene );
    
    /** Destructor */
    virtual ~SpotLight();
    
    virtual void draw() const;
    virtual void apply();
    
    const Vector3D &getSpotDirection() const { return _spotDirection; }
    virtual void setSpotDirection( const Vector3D &spotDirection );
    virtual void lookAt( const Point &target );
    
    const float getSpotExponent() const { return _spotExponent; }
    void setSpotExponent( const float spotExponent );
    
    const short getSpotCutoff() const { return _spotCutoff; }
    virtual void setSpotCutoff( const short spotCutoff );
    
protected:
    Vector3D _spotDirection;
    float _spotExponent;
    short _spotCutoff;
    
}; // class SpotLight


GAME_NAMESPACE_END


#endif // GRAPHICSLAB_SPOTLIGHT_HPP
