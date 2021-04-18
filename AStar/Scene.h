#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <unordered_map>

#include "Node.h"

enum SceneState {
	MENU,
	SCENE
};

class Scene {
public:
	// application state
	SceneState State;
	unsigned int Width, Height;
	unsigned int NTilesRow, NTilesColumn;
	int pickedID;


	// array of nodes to be drawn
	std::vector<Node> Nodes;

	// output of A* algorithm, used for drawing path
	std::unordered_map<Node, Node> CameFrom;
	std::unordered_map<Node, double> CostSoFar;

	// constructor/destructor
	Scene(unsigned int width, unsigned int height, unsigned int nTilesRow, unsigned int nTilesColumn);
	~Scene();

	// initialize application state (load all shaders / textures / levels)
	void Init();

	// initialize nodes to be rendered
	void InitNodes();

	// application loop
	void ProcessInput(int button, int action, const bool keys[]);
	void Update(float dt);
	void DrawScene(GLFWwindow* window);
	void DrawSceneSelectionMode(GLFWwindow* window, double xpos, double ypos);
	void DrawLine(std::vector<Node> &path, GLFWwindow* window);
	void RenderText(GLFWwindow* window);

	// reset
	void ResetLevel();

};

