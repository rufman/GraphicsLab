/**
 * sky.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   16.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_WINDOWMANAGER_HPP
#define GRAPHICSLAB_WINDOWMANAGER_HPP

// Includes
#include "defs.hpp"
#include "map.hpp"
#include "tgaTexture.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN

class Map ;	

/** Draws sky */
class Sky {
	public:
		Sky ( Map* map ) ;
		~Sky () ;
		void draw () ;
		void update ( float duration ) ;
	
	private:
		Map*		_map ;
		TGATexture* _skyTexture ;
		float		_rotationAngle ;
	
} ;

GRAPHICSLAB_NAMESPACE_END

#endif