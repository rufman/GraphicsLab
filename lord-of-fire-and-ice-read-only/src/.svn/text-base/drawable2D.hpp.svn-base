/*
 *  Drawable2D.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef  GRAPHICSLAB_DRAWABLE2D_HPP
#define GRAPHICSLAB_DRAWABLE2D_HPP

//Includes
#include "defs.hpp"
#include "gl_includes.hpp"
#include "drawable.hpp"
#include "vector.hpp"
#include "tgaTexture.hpp"
//Defines
typedef void* Drawable2Dlist;

GRAPHICSLAB_NAMESPACE_BEGIN
typedef
class Drawable2D : public Drawable {
public:
	
	Drawable2D(Scene& scene_);
	//~Drawable2D();
	void setPosition(uint x_, uint y_){ _x = x_; _y = y_;};
	void setSize(uint width_, uint height_){_height = height_; _width = width_;};
	void setSurface(TGATexture* tgaTexture_){_surface = tgaTexture_;};
	
	void draw();
	void update();
private:
	uint _width;
	uint _height;
	
	TGATexture* _surface;
	
};

typedef std::vector<Drawable2D*> Drawable2DList;


GRAPHICSLAB_NAMESPACE_END
#endif