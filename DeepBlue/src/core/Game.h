#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <string>
#include <memory>
#include <vector>

#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"
#include "../vendor/stb_image/stb_image.h"

#include "../util/Log.h"

#include "../scenes/IScene.h"
#include "../scenes/DemoScene.h"
#include "../scenes/LowScene.h"
#include "../scenes/MediumScene.h"
#include "../scenes/HighScene.h"

#include "SceneNode.h"
#include "Input.h"

class Game {
public:
	Game(const char* window_title, int width, int height, bool fullscreen);
	~Game();

	bool shouldClose();
	void init();
	void input();
	void update();
	void render();

	void setTitle(const char* title);
	void setWidth(int w) { m_width = w; }
	void setHeight(int h) { m_height = h; }

private:
	//Game Properties
	const char* m_window_title;
	int m_width;
	int m_height;
	bool m_isFullscreen;

	//GLFW Variables
	GLFWmonitor* m_monitor = nullptr;
	GLFWwindow* m_window = nullptr;
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

};