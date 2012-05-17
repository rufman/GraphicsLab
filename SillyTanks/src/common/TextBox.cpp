/**
 * textBox.cpp
* This class creates a text that can be displayed in a scene.
 */

// Class declaration include
#include "TextBox.hpp"

// common includes
#include "../common/GLIncludes.hpp"


namespace game_space {


TextBox::TextBox( Scene &scene, const std::string &text, float height, float lineWidth ) : 
    Drawable( scene ),
    _text( text ),
    _height( height ),
    _lineWidth( lineWidth )
{
}

TextBox::~TextBox()
{
}

float TextBox::getWidth() const
{	
    float textHeight = 120.0;
	float textWidth = 0;
    for ( uint i = 0; i < _text.length(); i++ )
    {
        textWidth += glutStrokeWidth( GLUT_STROKE_ROMAN, _text[i] );
    }
	textWidth *= _height/textHeight;
	return textWidth;
}

void TextBox::draw() const
{
    // Remember original line width
    float lineWidth;
    glGetFloatv( GL_LINE_WIDTH, &lineWidth );
    
    glLineWidth( _lineWidth );
    
    float textHeight = 120.0;
    float scaleFactor = _height/textHeight;
    
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glScalef( scaleFactor, scaleFactor, scaleFactor);
    
    for ( uint i = 0; i < _text.length(); i++ )
    {
        glutStrokeCharacter( GLUT_STROKE_ROMAN, _text[i] );
    }
    
    glPopMatrix();
    
    // Restore line width
    glLineWidth( lineWidth );
}


}
