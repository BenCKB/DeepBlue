#include "core/Game.h"
#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image/stb_image.h"

/* Entry Point */
int main() {

	/* Variables for game loop */
	int frames = 0;
	double currentUpdate = glfwGetTime();
	double lastUpdate = glfwGetTime();

	/* Set window properties */
	int width = 1320; //1320
	int height = 780; //780
	bool isFullscreen = false;

	/* Create Game object and initialize it */
	Game game("Test Rendering", width, height, isFullscreen);

	/* Main game loop */
	while (!game.shouldClose()) {

		/* Set variables needed for determining game refresh rate */
		currentUpdate = glfwGetTime(); //Returns time in seconds, with micro- or nanosecond resolution, depending on system
		frames++;

		/* When a second has passed, update time taken */
		if (currentUpdate - lastUpdate >= 1.0) {
			std::string title = "OpenGL Renderer - " + std::to_string(1000.0 / double(frames)) + " ms (" + std::to_string(frames) + " FPS)";
			game.setTitle(title.c_str());
			frames = 0;
			lastUpdate = currentUpdate;
		}

		//Handle input, update and rendering
		//TODO: Separate rendering from input and update call
		game.input();
		game.update();
		game.render();
	}
}
