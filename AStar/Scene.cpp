#include "Scene.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Node.h"

SpriteRenderer		*Renderer;


Scene::Scene(unsigned int width, unsigned int height, unsigned int nTilesRow, unsigned int nTilesColumn) 
	: Width(width), Height(height), NTilesRow(nTilesRow), NTilesColumn(nTilesColumn)
{
}

Scene::~Scene()
{
	delete Renderer;
}

void Scene::Init()
{
	// load shaders
	ResourceManager::LoadShader("Shaders/sprite.vert", "Shaders/sprite.frag", "sprite");

	// configure shaders
	glm::mat4 projection = glm::ortho(
		0.0f,
		static_cast<float>(this->Width),
		static_cast<float>(this->Height),
		0.0f,
		-1.0f,
		1.0f
	);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	// load textures
	ResourceManager::LoadTexture("Resources/block.png", false, "block");

	// set render specific controls
	Shader sprite = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(sprite);
}

void Scene::ProcessInput(float dt)
{
}

void Scene::Update(float dt)
{
}

void Scene::DrawScene()
{
	// calculate tile dimensions
	unsigned int unit_width = this->Width / static_cast<float>(NTilesColumn);
	unsigned int unit_height = this->Height / static_cast<float>(NTilesRow);

	for (unsigned int y = 0; y < NTilesRow; ++y) {
		for (unsigned int x = 0; x < NTilesColumn; ++x) {
			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec2 size(unit_width, unit_height);
			Node node(pos, size, ResourceManager::GetTexture("block"), glm::vec3(0.8, 0.8, 0.7));
			node.DrawNode(*Renderer);
		}
	}
}

void Scene::ResetLevel()
{
}
