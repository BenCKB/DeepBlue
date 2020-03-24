#include "Game.h"

std::shared_ptr<IScene> scene;

void renderImGui();

Game::Game(const char* title, int w, int h, bool fullscreen){
	m_window_title = title;
	m_width = w;
	m_height = h;
	m_isFullscreen = fullscreen;

	init();
}

void Game::setTitle(const char* title) {
	m_window_title = title;
	glfwSetWindowTitle(m_window, title);
}

Game::~Game(){

	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	m_monitor = nullptr;
	m_window = nullptr;
	m_window_title = nullptr;
}

bool Game::shouldClose(){
	return glfwWindowShouldClose(m_window);
}

void Game::init() {

	//Initialize Logger
	Log::init();
	LOG_INFO("Initializing engine...");
	
	//Initialize GLFW
	if (!glfwInit()) {
		LOG_CRITICAL("Error initializing GLFW");
		return;
	}

	//Set fullscreen variable
	m_isFullscreen ? m_monitor = glfwGetPrimaryMonitor() : m_monitor = NULL;

	//Create window
	m_window = glfwCreateWindow(m_width, m_height, m_window_title, m_monitor, NULL);

	if (!m_window){
		glfwTerminate();
		LOG_CRITICAL("Error creating GLFW window");
		return;
	}

	//Make the window's context current
	glfwMakeContextCurrent(m_window);

	//Don't limit the swap speed
	glfwSwapInterval(0);

	/* Initialize GLAD */
	if (!gladLoadGL()) {
		LOG_CRITICAL("Error initializing GLAD");
		return;
	}

	//GLFW callback functions
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetCursorPosCallback(m_window, cursor_position_callback);

	//Init low setting scene
	scene = std::make_shared<HighScene>();

	/*  === Set OpenGL variables ===  */
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	//Depth testing
	glEnable(GL_DEPTH_TEST);
	//Back face Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//Alpha blending
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Create ImGui Context and settings
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui::StyleColorsDark();

	Input::getInstance()->init();

	LOG_INFO("Successfully initialized engine.");
}

void Game::input(){
	std::shared_ptr<Camera> camera = scene->getCamera();
	Input* i = Input::getInstance();
	float force = i->getForce(); //1.0f

	glfwPollEvents();
	if (i->isKeyDown(GLFW_KEY_I)) {
		i->setForce(i->getForce() + 0.009f);
	}
	if (i->isKeyDown(GLFW_KEY_O)) {
		if(i->getForce() >= 0.01)
			i->setForce(i->getForce() - 0.009f);
	}
	if (i->isKeyDown(GLFW_KEY_W)) {
		camera->translate(camera->lookAt, force);
	}
	if (i->isKeyDown(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	}
	if (i->isKeyDown(GLFW_KEY_S)) {
		camera->translate(camera->lookAt * -1, force);
	}
	if (i->isKeyDown(GLFW_KEY_D)) {
		camera->translate(cross(camera->lookAt, camera->up).normalize(), force);
	}
	if (i->isKeyDown(GLFW_KEY_A)) {
		camera->translate((cross(camera->lookAt, camera->up) * -1).normalize(), force);
	}

	if (i->isKeyDown(GLFW_KEY_UP)) {
		camera->rotateX(force/10.0f);
	}

	if (i->isKeyDown(GLFW_KEY_DOWN)) {
		camera->rotateX(-force/10.0f);
	}

	if (i->isKeyDown(GLFW_KEY_LEFT)) {
		camera->rotateY(force/10.0f);
	}

	if (i->isKeyDown(GLFW_KEY_RIGHT)) {
		camera->rotateY(-force/10.0f);
	}
}

void Game::update(){
	scene->update();
}

void Game::render(){

	//Clear Colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene->render();
	
	renderImGui();

	//Swap front and back buffers
	glfwSwapBuffers(m_window);
}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	Input::getInstance()->setKey(key, scancode, action, mods);
}

void Game::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {}

void renderImGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("DeepBlue Engine");
	{
		ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		if (ImGui::CollapsingHeader("Information")) {
			ImGui::Text("Hardware Vendor: %s", glGetString(GL_VENDOR));
			ImGui::Text("GPU: %s", glGetString(GL_RENDERER));
			ImGui::Text("OpenGL Version: %s", glGetString(GL_VERSION));
			ImGui::Text("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
		}

		if (ImGui::CollapsingHeader("Scene Graph")) {
			std::vector<std::shared_ptr<SceneNode> > entities = scene->getAllEntities();
			if (ImGui::TreeNode("root")) {
				for (std::shared_ptr<SceneNode> entity : entities) {
					if (ImGui::TreeNode(entity->getName().c_str())) {
						std::cout << "The address is " << entity.get() << std::endl;
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
	}

	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}