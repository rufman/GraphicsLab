/*
 * dock.hpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#ifndef  GRAPHICSLAB_DOCK_HPP
#define GRAPHICSLAB_DOCK_HPP

//Includes
#include "defs.hpp"
#include "drawable.hpp"
#include "player.hpp"
#include "scene.hpp"
#include "drawable2D.hpp"
#include <vector>

GRAPHICSLAB_NAMESPACE_BEGIN

class Bar : public Drawable2D 
{
public:
	Bar(Scene& scene_, float scaleFactor_, TGATexture* surface_);
	void draw();
	void setTextureValues(int x_, int y_, int w_, int h_);
	void update(float percent_);
	void setPosition(int x_, int y_);
	void setDimension(int w_, int h_);
private:
	
	int _x;
	int _y;
	int _width;
	int _height;
	float _scaleFactor;
	float _tx, _ty,_tw,_th;
	TGATexture* _surface;
	float _percent; //How much the bar is filld
};



class WaponSlot : public Drawable2D {
public:
	WaponSlot(Scene& scene_, float scaleFactor_, TGATexture* surface_);
	void draw();
	void setTextureValues(int x_, int y_, int w_, int h_);
	void update(float percent_);
	void setPosition(int x_, int y_);
	void setDimension();
	void setActive(bool b_);
private:
	int _x,_y;
	int _width,_height;
	float _scaleFactor;
	
	bool _active;
	
	TGATexture* _surface;
	float _tx, _ty,_tw,_th;
};

class Dock: public Drawable2D
{
public:
	
	Dock(Scene& scene_);
	
	void draw();
	void update();
	
private:
	
	uint _x;
	uint _y;
	uint _width;
	uint _height;
	float _scaleFactor;
	TGATexture* _surface;
	
	Drawable2DList _components;
	
	float t1, t2,t3,t4;
	
	//Components
	Bar* _lifeBar;
	WaponSlot* _waponSlot1;
	WaponSlot* _waponSlot2;
	WaponSlot* _waponSlot3;
	WaponSlot* _waponSlot4;
	WaponSlot* _waponSlot5;
	WaponSlot* _activeSlot;
	
//	MiniMap	_miniMap;
//	SpellSelection _spellSelection;
//	WaponSelection _WaponSelection; 
//	QuickAccess _quickAccess;
	
};


GRAPHICSLAB_NAMESPACE_END

#endif
