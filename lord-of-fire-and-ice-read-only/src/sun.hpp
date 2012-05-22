/**
 * sun.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_SUN_HPP
#define GRAPHICSLAB_SUN_HPP

//Includes
#include "defs.hpp"
#include "scene.hpp"
#include "map.hpp"
#include "directionalLight.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN

class Sun {

public:

    Sun(Scene &scene, Map* map);

    ~Sun();

    static void apply();

    static void updatePosition();

    void draw();

    static void setAngle(float angle);

    static float getAngle();
    
private:

    Scene& _scene;
    static Map* _map;
    static float _angle;
    static DirectionalLight* _light;
	TGATexture* _texture;

};

GRAPHICSLAB_NAMESPACE_END

#endif  //  GRAPHICSLAB_SUN_HPP





