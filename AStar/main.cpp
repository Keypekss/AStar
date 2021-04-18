#include <iostream>
#include <algorithm>
#include <vector>

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// opengl configuration
	// --------------------
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_FUNC);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// initialize scene
	scene.Init();
	scene.InitNodes();
	scene.RenderText(window);
	

	while (!glfwWindowShouldClose(window)) {	

		glfwPollEvents();

		/*
		* scene this rendered after every mouse input
		*/
	}
}

// stores the buttons pressed in an array
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

	if (key == GLFW_KEY_SPACE && GLFW_PRESS) {
		scene.State = SCENE;
		scene.DrawScene(window);
		glfwSwapBuffers(window);
	}
}

// the scene is rendered again only after the type of a node has been changed
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (action == GLFW_RELEASE && scene.State == SCENE) {
		scene.DrawSceneSelectionMode(window, xpos, ypos);
		scene.ProcessInput(button, action, keys);
		scene.DrawScene(window);
		AStarPathFinder::PathFinder(scene.Nodes, scene.CameFrom, scene.CostSoFar);
		scene.DrawScene(window); // need to run this again for visited nodes to be rendered correctly
		std::vector<Node> path = AStarPathFinder::Reconstruct_Path(scene.Nodes, scene.CameFrom);
		scene.DrawLine(path, window);
	}	
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}