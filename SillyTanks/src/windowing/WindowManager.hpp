/**
 * windowManager.hpp
 * See windowManager.cpp
 */

#ifndef GRAPHICSLAB_WINDOWMANAGER_HPP
#define GRAPHICSLAB_WINDOWMANAGER_HPP

// Global definitions include
#include "../common/Definitions.hpp"

// Includes
#include "Window.hpp"

GAME_NAMESPACE_BEGIN

/** Class handles windows through GLUT */
class WindowManager
{
public:
	/**
	 * Get the singleton instance.
	 *
	 * @return      Singleton instance reference
	 */
	static WindowManager &getInstance();

	/**
	 * Create a new window.
	 *
	 * @param[in, out]  parameters      Window parameters
	 *
	 * @return          Pointer to created window
	 */
	Window *createWindow( Window::Parameters &parameters );

	/**
	 * Delete a window.
	 *
	 * @param[in, out]  window      Window to destroy
	 */
	void deleteWindow( Window &window );

	/**
	 * Get the currently active window.
	 *
	 * @return      Currently active window.
	 */
	Window *getActiveWindow() const {return _activeWindow;}

private:
	/** Constructor */
	WindowManager();

	/** Destructor */
	~WindowManager();

private:
	/** Singleton instance */
	static WindowManager *_instance;

private:
	/** Currently active window */
	Window *_activeWindow;

}; // class WindowManager

GAME_NAMESPACE_END

#endif // GRAPHICSLAB_WINDOWMANAGER_HPP
