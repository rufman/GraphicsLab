/*
 * dock.cpp
 *
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

#include "dock.hpp"
#include "inputManager.hpp"

GRAPHICSLAB_NAMESPACE_BEGIN
Bar::Bar(Scene& scene_, float scaleFactor_, TGATexture* surface_)
:Drawable2D( scene_),
_percent(0.5)

{
	_scaleFactor = scaleFactor_;
	_surface = surface_;
}

void Bar::setTextureValues(int x_, int y_, int w_, int h_)
{
	_tx = x_;
	_ty = y_;
	_tw = w_;
	_th = h_;
}

void Bar::setPosition(int x_, int y_)
{
	_x = x_ * _scaleFactor;
	_y = y_ * _scaleFactor;
}

void Bar::setDimension(int w_, int h_)
{
	_width = w_*_scaleFactor;
	_height = h_*_scaleFactor;
}


void Bar::draw()
{
	
	
	glPushMatrix();
	// Draw Surface
	_surface->setActive(true);
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(_tx, (_ty +(1-_percent)*_th));
	glVertex2f(_x, _y +_height*(1-_percent));
	
	glTexCoord2f(_tx , _ty+ _th);
	glVertex2f(_x, _y);
	
	glTexCoord2f(_tx + _tw , _ty+ _th);
	glVertex2f( _x + _width, _y);
	
	glTexCoord2f(_tx + _tw , (_ty +(1-_percent)*_th));
	glVertex2f(_x + _width, _y + _height*(1-_percent));
	glEnd();
	_surface->setActive(false);
	glPopMatrix();
}

void Bar::update(float percent_)
{
	_percent = percent_ <1 ? 1:percent_;
}

///Waponslot

WaponSlot::WaponSlot(Scene& scene_, float scaleFactor_, TGATexture* surface_)
:Drawable2D( scene_),
_active(false)
{
	_scaleFactor = scaleFactor_;
	_surface = surface_;
}


void WaponSlot::setTextureValues(int x_, int y_, int w_, int h_)
{
	_tx = x_;
	_ty = y_;
	_tw = w_;
	_th = h_;
}

void WaponSlot::setPosition(int x_, int y_)
{
	_x = x_ * _scaleFactor;
	_y = y_ * _scaleFactor;
}

void WaponSlot::setDimension()
{
	_width = _tw*_scaleFactor;
	_height = _th*_scaleFactor;
}

void WaponSlot::setActive(bool b_)
{
	this->_active = b_;
}

void WaponSlot::draw()
{
	glPushMatrix();
	// Draw Background

	glColor4f(0, 0, 0, 0.5);
	glBegin(GL_QUADS);
	glVertex2f(_x, _y + _height);
	glVertex2f(_x, _y);	
	glVertex2f( _x + _width, _y);
	glVertex2f(_x + _width, _y +_height) ;
	glEnd();
	
	// Draw Wapon
	//_surface->setActive(true);
	//glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);
	//glTexCoord2f(_tx, _ty);
	glVertex2f(_x, _y +_height);
	
	//glTexCoord2f(_tx , _ty+ _th);
	glVertex2f(_x, _y);
	
	//glTexCoord2f(_tx + _tw , _ty+ _th);
	glVertex2f( _x + _width, _y);
	
	//glTexCoord2f(_tx + _tw , _ty);
	glVertex2f(_x + _width, _y + _height);
	glEnd();
	//_surface->setActive(false);
	
	//Gray out or highlight
	if (_active) {
			int radius = 5;
			
		glColor4f(1, 1, 0, 0.5);
		glBegin(GL_QUADS);
			glColor4f(1, 1, 0, 0);
			glVertex2f(_x - radius, _y + _height + radius);
		glColor4f(1, 1, 0, 0.5);
			glVertex2f(_x , _y + _height);
			glVertex2f(_x + _width , _y + _height);
		glColor4f(1, 1, 0, 0.);
			glVertex2f(_x + _width + radius, _y + _height + radius);
			
			glVertex2f(_x - radius, _y + _height + radius);
			glVertex2f(_x - radius, _y-radius);
		glColor4f(1, 1, 0, 0.5);
			glVertex2f(_x , _y);
			glVertex2f(_x , _y + _height);

		glColor4f(1, 1, 0, 0.);
		glVertex2f(_x - radius, _y-radius);
		glVertex2f( _x+radius + _width, _y-radius);
		glColor4f(1, 1, 0, 0.5);
		glVertex2f( _x + _width, _y);
		glVertex2f(_x , _y);

		glColor4f(1, 1, 0, 0.5);
		glVertex2f( _x + _width, _y);
		glColor4f(1, 1, 0, 0.);
		glVertex2f( _x+radius + _width, _y-radius);
		glVertex2f(_x + _width + radius , _y + _height + radius);
		glColor4f(1, 1, 0, 0.5);
		glVertex2f(_x + _width  , _y + _height);

		glEnd();
	}
	
	glPopMatrix();
}

////DOCK
Dock::Dock(Scene& scene_)
:Drawable2D( scene_),
_height(180),
_width(452),
_x(10),
_y(10)
{	
	
	_surface = new TGATexture("data/textures/dock.tga");
	_surface->init();
	
	//Values for texture
	t1 = 0;
	t2 = 0;
	t3 = 635;
	t4 = 1595;
	
	_scaleFactor = _width/t4;
	
	//Setup Life Bar
	_lifeBar = new Bar(_scene, _scaleFactor, _surface);
	_lifeBar->setTextureValues(29, 637, 344, 486);
	_lifeBar->setPosition((46) , (635 - 65 - 486) );
	_lifeBar->setDimension(344,486);
	_components.push_back(_lifeBar);
	
	//Set up Wapon Slots
	_waponSlot1 = new WaponSlot(_scene, _scaleFactor, _surface);
	_waponSlot1->setTextureValues(211, 469, 198, 198);
	_waponSlot1->setPosition(469 , 635 - 198 -211 );
	_waponSlot1->setDimension();
	_components.push_back(_waponSlot1);
	
	_waponSlot2 = new WaponSlot(_scene, _scaleFactor, _surface);
	_waponSlot2->setTextureValues(211, 469, 198, 198);
	_waponSlot2->setPosition(698 , 635 - 198 -211 );
	_waponSlot2->setDimension();
	_components.push_back(_waponSlot2);
	
	_waponSlot3 = new WaponSlot(_scene, _scaleFactor, _surface);
	_waponSlot3->setTextureValues(211, 469, 198, 198);
	_waponSlot3->setPosition(917 , 635 - 198 -211 );
	_waponSlot3->setDimension();
	_components.push_back(_waponSlot3);
	
	_waponSlot4 = new WaponSlot(_scene, _scaleFactor, _surface);
	_waponSlot4->setTextureValues(211, 469, 198, 198);
	_waponSlot4->setPosition(1142 , 635 - 198 -211 );
	_waponSlot4->setDimension();
	_components.push_back(_waponSlot4);
	
	_waponSlot5 = new WaponSlot(_scene, _scaleFactor, _surface);
	_waponSlot5->setTextureValues(211, 469, 198, 198);
	_waponSlot5->setPosition(1369 , 635 - 198 -211 );
	_waponSlot5->setDimension();
	_components.push_back(_waponSlot5);
	
	_activeSlot = _waponSlot1;

}



void Dock::draw()
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	
	
	
	//Set position
	//updateOffset()
	glTranslated(_x, _y, 0);

	_surface->setActive(true);
	
	// Draw Background
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);

	glTexCoord2f(t1, t2);
	glVertex2f(0.0, _height);

	
	glTexCoord2f(t1, t3);
	glVertex2f(0, 0);
	
	glTexCoord2f(t4, t3);
	glVertex2f( _width,0);
	
	glTexCoord2f(t4, t2);
	glVertex2f(_width, _height);
	glEnd();
	_surface->setActive(false);
	
	_lifeBar->draw();
	_waponSlot1->draw();
	_waponSlot2->draw();
	_waponSlot3->draw();
	_waponSlot4->draw();
	_waponSlot5->draw();
	
}

void Dock::update()
{	
	for (int i = 0; i < _components.size(); i++) 
	{
		_components.at(i)->update();
	}
	
	if (InputManager::keyUp('1')) 
	{
		_activeSlot->setActive(false);
		_waponSlot1->setActive(true);
		_activeSlot = _waponSlot1;
		}
		else if ((InputManager::keyUp('2')))
		{
			_activeSlot->setActive(false);
			_waponSlot2->setActive(true);
			_activeSlot = _waponSlot2;
		}
		else if ((InputManager::keyUp('3')))
		{
			_activeSlot->setActive(false);
			_waponSlot3->setActive(true);
			_activeSlot = _waponSlot3;
		}
		else if ((InputManager::keyUp('4')))
		{
			_activeSlot->setActive(false);
			_waponSlot4->setActive(true);
			_activeSlot = _waponSlot4;
		}
		else if ((InputManager::keyUp('5')))
		{
			_activeSlot->setActive(false);
			_waponSlot5->setActive(true);
			_activeSlot = _waponSlot5;
		}
	}
GRAPHICSLAB_NAMESPACE_END
