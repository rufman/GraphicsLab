#ifndef GAME_APPLICATION_HPP
#define GAME_APPLICATION_HPP

// Global definitions include
#include "../common/Definitions.hpp"

// std includes
#include <string>

namespace game_space {

// Forward declarations
class Window;

/** Class for an application */
class Application {
public:
	// Parameters
	struct Parameters {
		// Title of the application window
		std::string windowTitle;

		//Texture file for the sky
		std::string skyTextureFile;

		//terrain file prefix for the ground
		std::string terrainFilePrefix;

		//water height
		float waterHeight;

		float fogDensity;

		float fogStart;
		float fogEnd;
		float fogRed;
		float fogGreen;
		float fogBlue;

		// Constructor of parameters
		Parameters();

		// Parse the application parameters
		void parse(int argc, char **argv);
	};

	/**
	 * Get the singleton instance.
	 *
	 * @return      Singleton instance reference
	 */
	static Application &getInstance();

	/**
	 * Get application parameters.
	 *
	 * @return      Parameters
	 */
	const Parameters &getParameters() const {
		return _parameters;
	}

	/**
	 * Initialize application.
	 *
	 * @param[in]   argc    Command line argument count
	 * @param[in]   argv    Command line arguments
	 */
	void initialize(int argc, char **argv);

	/**
	 * Run application main loop.
	 *
	 * @return      Return status value
	 */
	int run();

private:

	// Constructor
	Application();

	// Destructor
	~Application();

	// Singleton instance
	static Application *_instance;

	// Application parameters
	Parameters _parameters;

	// Main window
	Window *_mainWindow;

};
// class Application
}
#endif // GRAPHICSLAB_APPLICATION_HPP
