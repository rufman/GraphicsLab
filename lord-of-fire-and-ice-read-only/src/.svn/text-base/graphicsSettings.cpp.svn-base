/**
 * graphicsSettings.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */


// Class declaration include
#include "graphicsSettings.hpp"

// Includes
#include "window.hpp"
#include "menuManager.hpp"
#include <cstdio>


GRAPHICSLAB_NAMESPACE_BEGIN

Window* GraphicsSettings::_window;

uchar GraphicsSettings::_sectorRenderLimit;
uchar GraphicsSettings::_sectorReflectionRenderLimit;

uchar GraphicsSettings::_highestQuality;

uchar GraphicsSettings::_reflections;

uchar GraphicsSettings::_shadows;

uchar GraphicsSettings::_grass;

uchar GraphicsSettings::_tempFullscreen;
uchar GraphicsSettings::_fullscreenAfterLoad;
uchar GraphicsSettings::_fullscreen;


GraphicsSettings::GraphicsSettings(Window* window) {
	_window = window;
    _sectorRenderLimit = 16;
    _sectorReflectionRenderLimit = 12;
    _highestQuality = 3;
    _reflections = 1;
    _shadows = 1;
    _grass = 1;
    _tempFullscreen = 0;
    _fullscreenAfterLoad = 0;
    _fullscreen = 0;
}


GraphicsSettings::~GraphicsSettings() {
}


void GraphicsSettings::save() {
	if(_fullscreen == 0 && _tempFullscreen == 1) {
		_window->fullscreenOn();
	} else if(_fullscreen == 1 && _tempFullscreen == 0) {
		_window->fullscreenOff();
	}
	FILE* file = fopen("data/settings.set", "wb");
	if(file != NULL) {
		fwrite(&_sectorRenderLimit, 1, 1, file);
		fwrite(&_sectorReflectionRenderLimit, 1, 1, file);
		fwrite(&_highestQuality, 1, 1, file);
		fwrite(&_reflections, 1, 1, file);
		fwrite(&_shadows, 1, 1, file);
		fwrite(&_grass, 1, 1, file);
		fwrite(&_fullscreen, 1, 1, file);
		fclose(file);
	}
}


void GraphicsSettings::load() {
	FILE* file = fopen("data/settings.set", "rb");
	if(file != NULL) {
		fread(&_sectorRenderLimit, 1, 1, file);
		fread(&_sectorReflectionRenderLimit, 1, 1, file);
		fread(&_highestQuality, 1, 1, file);
		fread(&_reflections, 1, 1, file);
		fread(&_shadows, 1, 1, file);
		fread(&_grass, 1, 1, file);
		fread(&_fullscreen, 1, 1, file);
		fclose(file);
		_tempFullscreen = _fullscreen;
		_fullscreenAfterLoad = _fullscreen;
	}
	MenuManager::updateViewDistanceButton();
	MenuManager::updateGeometryQualityButton();
	MenuManager::updateDynamicReflectionButton();
	MenuManager::updateDynamicShadowsButton();
	MenuManager::updateHerbageButton();
	MenuManager::updateFullscreenButton();
}


void GraphicsSettings::changeSectorRenderLimit() {
	if(_sectorRenderLimit == 16) {
		_sectorRenderLimit = 8;
		_sectorReflectionRenderLimit = 6;

	} else if(_sectorRenderLimit == 8) {
		_sectorRenderLimit = 12;
		_sectorReflectionRenderLimit = 8;

	} else if(_sectorRenderLimit == 12) {
		_sectorRenderLimit = 16;
		_sectorReflectionRenderLimit = 12;
	}
	MenuManager::updateViewDistanceButton();
}


uchar GraphicsSettings::getSectorRenderLimit() {
	return _sectorRenderLimit;
}


uchar GraphicsSettings::getSectorReflectionRenderLimit() {
	return _sectorReflectionRenderLimit;
}


void GraphicsSettings::changeHighestQuality() {
	if(_highestQuality == 3) {
		_highestQuality = 1;
	} else if(_highestQuality == 1) {
		_highestQuality = 2;
	} else if(_highestQuality == 2) {
		_highestQuality = 3;
	}
	MenuManager::updateGeometryQualityButton();
}


uchar GraphicsSettings::getHighestQuality() {
	return _highestQuality;
}


void GraphicsSettings::changeReflections() {
	if(_reflections == 1) {
		_reflections = 0;
	} else if(_reflections == 0) {
		_reflections = 1;
	}
	MenuManager::updateDynamicReflectionButton();
}


uchar GraphicsSettings::getReflections() {
	return _reflections;
}


void GraphicsSettings::changeShadows() {
	if(_shadows == 1) {
		_shadows = 0;
	} else if(_shadows == 0) {
		_shadows = 1;
	}
	MenuManager::updateDynamicShadowsButton();
}


uchar GraphicsSettings::getShadows() {
	return _shadows;
}


void GraphicsSettings::changeGrass() {
	if(_grass == 1) {
		_grass = 0;
	} else if(_grass== 0) {
		_grass = 1;
	}
	MenuManager::updateHerbageButton();
}


uchar GraphicsSettings::getGrass() {
	return _grass;
}


void GraphicsSettings::changeFullscreen() {
	if(_tempFullscreen == 0) {
		_tempFullscreen = 1;
	} else if(_tempFullscreen == 1) {
		_tempFullscreen = 0;
	}
	MenuManager::updateFullscreenButton();
}


uchar GraphicsSettings::getFullscreen() {
	return _fullscreen;
}

uchar GraphicsSettings::getTempFullscreen() {
	return _tempFullscreen;
}

void GraphicsSettings::setFullscreen(uchar value) {
	_fullscreen = value;
	_tempFullscreen = value;
	MenuManager::updateFullscreenButton();
}

void GraphicsSettings::checkIfFullscreen() {
	if(_fullscreenAfterLoad == 1) {
		_window->fullscreenOn();
	}
}


GRAPHICSLAB_NAMESPACE_END


