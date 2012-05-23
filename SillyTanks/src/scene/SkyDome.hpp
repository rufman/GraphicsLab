/**
 * skyDome.hpp
* See skyDome.cpp
 */

#ifndef GAME_SKYDOME_HPP
#define GAME_SKYDOME_HPP


// common include
#include "../common/Drawable.hpp"

#include <string>
#include <vector>

namespace game_space {

// Forward declarations
class TGATexture;

/** Sky Dome */
class SkyDome : public Drawable
{
public:
    /** 
	 * Constructor
	 *
	 * @param[in, out]	scene			Owning scene
	 * @param[in]		textureFile		Sky texture file
	 * @param[in]		radius			Radius of the dome
	 * @param[in]		numLongitudes	Number of longitudes
	 * @param[in]		numLatitudes	Number of latitudes
	 */
    SkyDome( Scene &scene, const std::string &textureFile, float radius, uint numLongitudes, uint numLatitudes );
	
	/** Destructor */
	~SkyDome();
	
    /** Draw the sky dome to the screen. */
    void draw() const;
	
	/** Reset sky parameters */
	void reset();
	
	/** 
	 * Update the sky for 'seconds' amount of time
	 *
	 * @param[in]	seconds		Time to update for
	 */
	void update( float seconds );
	
	/**
	 * Get the position of the sun on the sky
	 *
	 * @return		Sun position on the sky
	 */
	Point getSunPosition() const;

	float getDisplacement() const;

private:
	/** Radius of sky dome hepsiphere */
	float _radius;
	/** Number of vertical slices */
	uint _numLongitudes;
	/** Number of horizontal lines slicing the hemisphere, including the pole point */
	uint _numLatitudes;
	
	// Mesh data
	std::vector<Point> _vertices;
	std::vector<Vector3D> _vertexNormals;
	
	struct Quad
	{
		uint vertex1, vertex2, vertex3, vertex4;
		
		Quad( uint vertex1_, uint vertex2_, uint vertex3_, uint vertex4_ ) :
			vertex1( vertex1_ ), vertex2( vertex2_ ), vertex3( vertex3_ ), vertex4( vertex4_ ) {}
	};
	std::vector<Quad> _quads;
	std::vector<Vector3D> _quadNormals;
	
	/** Sky texture. Should be seamless */
	TGATexture *_texture;
	
	/** Variable to identify sky orientation */
	float _textureDisplacement;
	
}; // class SkyDome


}


#endif
