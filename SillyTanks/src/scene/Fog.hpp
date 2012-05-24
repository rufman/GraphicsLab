#ifndef GRAPHICSLAB_FOG_HPP
#define GRAPHICSLAB_FOG_HPP

// Includes
#include "Scene.hpp"

namespace game_space {


class Fog {

public:

	/** Constructor */
	Fog(float fogDensity, float fogStart, float fogEnd, float fogRed, float fogGreen, float fogBlue);

	/** Destructor */
	~Fog();

	void apply();
	void remove();

};


}

#endif /* GRAPHICSLAB_FOG_HPP */
