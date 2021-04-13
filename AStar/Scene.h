#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <unordered_map>

#include "Node.h"

class Scene {
public:
	// application state
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
	void ProcessInput(float dt);
	void Update(float dt);
	void DrawScene(GLFWwindow* window);
	void DrawSceneSelectionMode(GLFWwindow* window, double xpos, double ypos);
	

	// reset
	void ResetLevel();

};

