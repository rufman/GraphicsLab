/**
 * introScreen.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_INTROSCREEN_HPP
#define GRAPHICSLAB_INTROSCREEN_HPP

// Includes
#include "defs.hpp"
#include "scene.hpp"
#include "tgaTexture.hpp"
#include "camera2D.hpp";

GRAPHICSLAB_NAMESPACE_BEGIN

class IntroScreen {

public:

    IntroScreen(Scene &scene);
    
    ~IntroScreen();
    
    void draw();
    
    void update(float duration);

    static bool isOn();

    void loadingFinished();

private:

    static const float INTRO_SCREEN_DISPLAY_LENGTH = 5.0;

    Scene& _scene;

    TGATexture* _texture;
    Camera2D* _camera2D;

    bool _mainMenuLoaded;
    static bool _on;

    float _duration;

};

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_INTROSCREEN_HPP


