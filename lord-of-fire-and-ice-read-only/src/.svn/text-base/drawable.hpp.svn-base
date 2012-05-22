/**
 * drawable.hpp
 *
 * Author   :   Fatih Erol
 * Date     :   18.02.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_DRAWABLE_HPP
#define GRAPHICSLAB_DRAWABLE_HPP

// Includes
#include <vector>
#include "defs.hpp"
#include "types.hpp"
#include "gl_includes.hpp"
#include "vector.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN

// Forward declarations
class Scene;


class Drawable
{
public:
    /** Constructor. */
    Drawable(Scene& scene, float x, float y, float altitude) :
        _scene(scene),
        _x(x),
        _y(y),
        _altitude(altitude) {}
	
    
    /** Destructor */
    virtual ~Drawable() {}

    virtual void draw(int quality) {}

    float x() {
    	return _x;
    }

    float y() {
    	return _y;
    }

protected:
    Scene& _scene;

    float _x;
    float _y;
    float _altitude;
    
};

typedef std::vector <Drawable*> DrawableList;

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_DRAWABLE_HPP


