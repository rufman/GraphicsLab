/**
 * textBox.hpp
* See textBox.cpp
 */

#ifndef GRAPHICSLAB_TEXTBOX_HPP
#define GRAPHICSLAB_TEXTBOX_HPP


// Base class include
#include "../common/Drawable.hpp"
#include "../scene/Scene.hpp"

// Includes
#include <string>


namespace game_space {


/** Class to draw text to the screen */
class TextBox : public Drawable
{
public:
    /** 
     * Constructor 
     *
     * @param[in]   scene		Owning scene
     * @param[in]   text		Text
     * @param[in]   height		Height of text box, kind of font size
	 * @param[in]	lineWidth	Width of the line to draw text with
     */
    TextBox( Scene &scene, const std::string &text, float height = 1.0, float lineWidth = 1.0 );
    
    /** Destructor */
    ~TextBox();
    
    /** Draw the object to the screen. */
    void draw() const;
    
	/** 
	 * Get width of the drawn text.
	 *
	 * @return		Width of the draw text
	 */
	float getWidth() const;
    
private:
    // Text
    std::string _text;
    
    // Height of the text box
    float _height;
    
    // Line width
    float _lineWidth;
    
}; // class TextBox


}


#endif  // GRAPHICSLAB_TEXTBOX_HPP
