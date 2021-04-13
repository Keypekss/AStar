#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Scene.h"
#include "AStarPathFinder.h"

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// screen dimensions
const unsigned int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 800;

// store keys pressed to send them to mouse_button_callback
bool keys[512];

Scene scene(SCREEN_WIDTH, SCREEN_HEIGHT, 40, 40);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
	glfwSwapInterval(0); // disable vsync

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "AStar", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// glad: load all opengl function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// opengl configuration
	// --------------------
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_FUNC);

	// initialize scene
	scene.Init();
	scene.InitNodes();
	scene.DrawScene(window);

	// deltaTime variables
	// -------------------
	double deltaTime = 0.0f;
	double lastFrame = 0.0f;

// 	for (int i = 0; i < 5; i++) {
// 		std::cout << scene.Nodes.at(i).Position.x << " " << scene.Nodes.at(i).Position.y << std::endl;
// 	}

	while (!glfwWindowShouldClose(window)) {		

		// calculate deltaTime
		// -------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// manage user input
		// -----------------
		scene.ProcessInput(deltaTime);

		// render scene
		// ------------
		


		// render scene in selection mode
		
		
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// when a user presses the escape key, we set the WindowShouldClose property to true closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (keys[key] >= 0 && keys[key] <= 512) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	scene.DrawSceneSelectionMode(window, xpos, ypos);	

	if (keys[GLFW_KEY_LEFT_CONTROL] == true && button == GLFW_MOUSE_BUTTON_LEFT)
		scene.Nodes.at(scene.pickedID).Type = START;
	else if (keys[GLFW_KEY_LEFT_CONTROL] == true && button == GLFW_MOUSE_BUTTON_RIGHT)
		scene.Nodes.at(scene.pickedID).Type = GOAL;
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		scene.Nodes.at(scene.pickedID).Type = BLOCK;
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		scene.Nodes.at(scene.pickedID).Type = NORMAL;

	if (action == GLFW_RELEASE) {
		AStarPathFinder::PathFinder(scene.Nodes, scene.CameFrom, scene.CostSoFar);
		//AStarPathFinder::Reconstruct_Path(scene.Nodes, scene.CameFrom);

		for (auto &pair : scene.CameFrom) {
			std::cout << "First: " << pair.first.ID << " Second: " << pair.second.ID << std::endl;
		}
	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}