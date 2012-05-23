/*
 * Mirror.hpp
 *
 *  Created on: 22.05.2012
 *      Author: gregory
 */

#ifndef MIRROR_HPP_
#define MIRROR_HPP_

// commons includes
#include "../common/Drawable.hpp"
#include "../common/Types.hpp"
#include "../common/ScreenTexture.hpp"
#include "../common/Material.hpp"

namespace game_space {

class Material;

/** Water */
class Mirror: public Drawable {
public:
	/**
	 * Constructor
	 *
	 * @param[in, out]	scene	Owning scene
	 * @param[in]		width	Width of the sea
	 * @param[in]		length	Length of the sea
	 */
	Mirror(Scene &scene);

	/** Destructor */
	~Mirror();

	/** Draw the object to the screen. */
	void draw() const;

	/** Reset the sea parameters */
	void reset();

	/** Update sea surface for given duration */
	void update(float seconds);


	/**
	 * Set up OpenGL camera to imitate mirror, as if
	 * there is a camera behind the mirror glass.
	 */
	void applyCamera(Point tankPosition) ;

	/**
	 * Capture the drawn image after applyCamera() is called as
	 * the mirror texture, which can be applied to the mirror surface rectangle.
	 */
	void capture();


private:


	/** Dimensions of the sea */
	float _width, _length;


	ScreenTexture _texture;

};

}


#endif /* MIRROR_HPP_ */
