/**
 * loadingScreen.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_LOADINGSCREEN_HPP
#define GRAPHICSLAB_LOADINGSCREEN_HPP

// Includes
#include "defs.hpp"
#include "scene.hpp"
#include "tgaTexture.hpp"
#include "camera2D.hpp";

GRAPHICSLAB_NAMESPACE_BEGIN

class LoadingScreen {

public:

	LoadingScreen(Scene &scene);
    
    ~LoadingScreen();
    
    void initialize();

    static void draw();

    static bool isOn();

    void startLoading();

    void loadingFinished();

    static void setPercentage(float percentage);

private:

    Scene& _scene;

    static TGATexture* _texture;
    static Camera2D* _camera2D;

    static bool _on;

    static float _percentage;

};

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_LOADINGSCREEN_HPP


