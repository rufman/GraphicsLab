/**
 * graphicsSettings.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef GRAPHICSLAB_GRAPHICSSETTINGS_HPP
#define GRAPHICSLAB_GRAPHICSSETTINGS_HPP

//Includes
#include "defs.hpp"
#include "window.hpp"
#include "menuManager.hpp"


GRAPHICSLAB_NAMESPACE_BEGIN

class GraphicsSettings {

public:

	GraphicsSettings(Window* window);

    ~GraphicsSettings();

    static void save();

    static void load();

    static void changeSectorRenderLimit();
    static uchar getSectorRenderLimit();
    static uchar getSectorReflectionRenderLimit();
    
    static void changeHighestQuality();
    static uchar getHighestQuality();

    static void changeReflections();
    static uchar getReflections();

    static void changeShadows();
    static uchar getShadows();

    static void changeGrass();
    static uchar getGrass();

    static void changeFullscreen();
    static uchar getFullscreen();
    static uchar getTempFullscreen();
    static void setFullscreen(uchar value);

    static void checkIfFullscreen();

private:

    static Window* _window;

    static uchar _sectorRenderLimit;
    static uchar _sectorReflectionRenderLimit;

    static uchar _highestQuality;

    static uchar _reflections;

    static uchar _shadows;

    static uchar _grass;

    static uchar _tempFullscreen;
    static uchar _fullscreenAfterLoad;
    static uchar _fullscreen;

};

GRAPHICSLAB_NAMESPACE_END

#endif  //  GRAPHICSLAB_GRAPHICSSETTINGS_HPP





