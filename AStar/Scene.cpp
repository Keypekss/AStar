#include "Scene.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Node.h"

#include <sstream>
#include <algorithm>

SpriteRenderer	*Renderer;
SpriteRenderer	*SelectionRenderer;
SpriteRenderer	*LineRenderer;

Scene::Scene(unsigned int width, unsigned int height, unsigned int nTilesRow, unsigned int nTilesColumn) 
	: Width(width), Height(height), NTilesRow(nTilesRow), NTilesColumn(nTilesColumn), CameFrom(), CostSoFar()
{
}

Scene::~Scene()
{
	delete Renderer;
	delete SelectionRenderer;
	delete LineRenderer;
	Nodes.clear();
}

void Scene::Init()
{
	// load shaders
	ResourceManager::LoadShader("Shaders/sprite.vert", "Shaders/sprite.frag", "sprite");
	ResourceManager::LoadShader("Shaders/picking.vert", "Shaders/picking.frag", "picking");
	ResourceManager::LoadShader("Shaders/line.vert", "Shaders/line.frag", "line");

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
	ResourceManager::GetShader("picking").Use().SetMatrix4("projection", projection);
	ResourceManager::GetShader("line").Use().SetMatrix4("projection", projection);

	// load textures
	ResourceManager::LoadTexture("Resources/block.png", false, "block");

	// set render specific controls
	Shader sprite = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(sprite);
	Shader picking = ResourceManager::GetShader("picking");
	SelectionRenderer = new SpriteRenderer(picking);
	Shader line = ResourceManager::GetShader("line");
	LineRenderer = new SpriteRenderer(line);
}

void Scene::InitNodes()
{
	// calculate tile dimensions
	unsigned int unit_width = this->Width / static_cast<unsigned int>(NTilesColumn);
	unsigned int unit_height = this->Height / static_cast<unsigned int>(NTilesRow);

	for (unsigned int y = 0; y < NTilesRow; ++y) {
		for (unsigned int x = 0; x < NTilesColumn; ++x) {
			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec2 size(unit_width, unit_height);
			Node node(pos, size, ResourceManager::GetTexture("block"), glm::vec3(0.7, 0.85, 1.0));
			Node::SetColor(node);
			this->Nodes.push_back(node);
		}
	}
}

void Scene::ProcessInput(int button, int action, const bool keys[])
{
	auto startNode = std::find_if(Nodes.begin(), Nodes.end(), [](const Node &node) { return node.Type == START; });
	auto goalNode = std::find_if(Nodes.begin(), Nodes.end(), [](const Node &node) { return node.Type == GOAL; });
	bool b = Node::SetType(*startNode, *goalNode, Nodes.at(pickedID - 1), button, action, keys);
	if (b) {
		// reset previous visited nodes back to normal state before finding new nodes
		for (auto it = Nodes.begin(); it != Nodes.end();) {
			it = std::find_if(Nodes.begin(), Nodes.end(), [](const Node &node) { return node.Type == VISITED; });
			if (it == Nodes.end())
				break;
			it->Type = NORMAL;
			Node::SetColor(*it);
		}
		// clear unordered maps if any node type has changed to start over
		CameFrom.clear();
		CostSoFar.clear();
	}
}

void Scene::Update(float dt)
{
}

void Scene::DrawScene(GLFWwindow* window)
{
	const GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, color);
			
	for (unsigned int i = 0; i < Nodes.size(); i++) {
		Nodes.at(i).DrawNode(*Renderer);
	}
	
	Node::ResetCount();
	//glfwSwapBuffers(window);
}

// draws node with unique color behind the scenes to set pickID, use glSwapBuffers to see what's being drawn
void Scene::DrawSceneSelectionMode(GLFWwindow* window, double xpos, double ypos)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// calculate tile dimensions
	unsigned int unit_width = this->Width / static_cast<unsigned int>(NTilesColumn);
	unsigned int unit_height = this->Height / static_cast<unsigned int>(NTilesRow);

	unsigned int id = 1;
	for (unsigned int y = 0; y < NTilesRow; ++y) {
		for (unsigned int x = 0; x < NTilesColumn; ++x) {
			// convert "x*y", the integer mesh ID, into an RGB color			
			int r = (id & 0x000000FF) >> 0;
			int g = (id & 0x0000FF00) >> 8;
			int b = (id & 0x00FF0000) >> 16;
			id++;

			ResourceManager::GetShader("picking").Use();
			glm::vec4 colorID = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
			//std::cout << "rgb: " << colorID.r << " " << colorID.g << " " << colorID.b << std::endl;
			ResourceManager::GetShader("picking").SetVector4f("PickingColor", colorID);

			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec2 size(unit_width, unit_height);
			Node node(pos, size, ResourceManager::GetTexture("block"), glm::vec3(1.0, 1.0, 1.0));
			node.DrawNode(*SelectionRenderer);			
		}
	}
	glFlush();
	glFinish();	

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Read the pixel at the center of the screen.
	// You can also use glfwGetMousePos().
	// Ultra-mega-over slow too, even for 1 pixel, 
	// because the framebuffer is on the GPU.
	unsigned char data[4];
	glReadPixels(xpos, 800 - ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// Convert the color back to an integer ID
	pickedID =
		data[0] +
		data[1] * 256 +
		data[2] * 256 * 256;

	std::string message;
	if (pickedID == 0x00ffffff) { // Full white, must be the background !
		message = "background";
	} else {
		std::ostringstream oss;
		oss << "mesh " << pickedID;
		message = oss.str();
	}	
	//std::cout << message << std::endl;
	Node::ResetCount();
	//glfwSwapBuffers(window);
}

void Scene::DrawLine(std::vector<Node> &path, GLFWwindow* window)
{
	for (unsigned int i = 1; i < path.size(); i++) {
		LineRenderer->DrawLine(path.at(i - 1).Position, path.at(i).Position, 3, glm::vec3(1.0f, 1.0f, 0.0f));
	}	
	glfwSwapBuffers(window);
}

void Scene::SetVisited()
{
	for (auto &it : CameFrom) {
		Nodes.at(it.first.ID).SetVisited();
	}
}

void Scene::ResetLevel()
{
}
