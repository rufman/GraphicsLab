/*
 * fog.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef FOG_HPP_
#define FOG_HPP_

// Includes
#include "defs.hpp"
#include "scene.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN


class Fog {

public:

	/** Constructor */
	Fog(Scene& scene);

	/** Destructor */
	~Fog();

	/** Draw sector */
	void apply();

private:
	Scene& _scene;
};


GRAPHICSLAB_NAMESPACE_END

#endif /* FOG_HPP_ */
