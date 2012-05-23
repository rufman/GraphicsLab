/**
 * Turret.hpp
* See turret.cpp
 */

#ifndef GAME_TURRET_HPP
#define GAME_TURRET_HPP


// Base class include
#include "../../../common/Drawable.hpp"

// common includes
#include "../../../common/Types.hpp"
#include "../../../common/PLYModel.hpp"


namespace game_space {


/** Cannon */
class Turret : public Drawable
{
public:
    /** 
	 * Constructor
	 *
	 * @param[in, out]	scene	Owning scene
	 */
    Turret( Scene &scene );
    
    /** Destructor */
    ~Turret();
    
    /** Draw the object to the screen. */
    void draw() const;
    
    /** Reset the cannon parameters */
    void reset();
    
    // Get/Set cannon parameters    
    float getElevation() const;
    void setElevation( float elevation );

    float getAzimuth() const;
    void setAzimuth( float azimuth );
        
    float getPower() const ;
    void setPower( float power );
    void load( const std::string &modelFile, const std::string &textureFile);

    /**
     * Return the coordinates of the tip of the cannon muzzle with reference to cannon base
     *
     * @return      Coordinates of the tip of the cannon muzzle
     */
    Point getMuzzlePosition() const;
    
    Point getPosition() const;
    void setPosition(Point position);


public:
    // Muzzle parameters
    static const float _baseWidth;
    
private:    
    // Elevation of the muzzle from y = 0 plane
    float _elevation;
	
    // Azimuth angle of the muzzle from x = 0 plane
	float _azimuth;
	
    // Projectile throwing power of the cannon
    float _power;

    // current position of the cannon
    Point _position;

    PLYModel* _turret;

}; // class Cannon


}
#endif
