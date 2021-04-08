#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Scene {
public:
	// application state
	unsigned int Width, Height;
	unsigned int NTilesRow, NTilesColumn;

	// constructor/destructor
	Scene(unsigned int width, unsigned int height, unsigned int nTilesRow, unsigned int nTilesColumn);
	~Scene();

	// initialize application state (load all shaders / textures / levels)
	void Init();

	// application loop
	void ProcessInput(float dt);
	void Update(float dt);
	void DrawScene();
	

	// reset
	void ResetLevel();

};

