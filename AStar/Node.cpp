#include "Node.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

#include <algorithm>

#include <GLFW/glfw3.h>

static unsigned int count = 0;

Node::Node() 
	: Position(0.0f, 0.0f), Size(1.0f, 1.0f), Color(1.0f), Rotation(0.0f), Type(NORMAL), ID(0), Sprite()
{
	ID = ++count;
}

Node::Node(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color)
	: Position(pos), Size(size), Color(color), Rotation(0.0f), Type(NORMAL), ID(0), Sprite(sprite)
{
	ID = ++count;
	if (ID == 610)
		Type = START;
	if (ID == 630)
		Type = GOAL;
}

Node::~Node()
{
	
}

void Node::DrawNode(SpriteRenderer & renderer)
{
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}

void Node::ResetCount()
{
	count = 0;
}

void Node::SetColor(Node &node)
{
	if		(node.Type == NORMAL)
		node.Color = glm::vec3(0.7, 0.85, 1.0);
	else if (node.Type == BLOCK)
		node.Color = glm::vec3(0.45f, 0.45f, 0.45f);
	else if (node.Type == START)
		node.Color = glm::vec3(0.67f, 0.85f, 0.14f);
	else if (node.Type == GOAL)
		node.Color = glm::vec3(0.90f, 0.0f, 0.36f);
	else if (node.Type == VISITED)
		node.Color = glm::vec3(0.0f, 0.45f, 0.90f);
}

bool Node::SetType(Node &start, Node &goal, Node &node, int button, int action, const bool keys[])
{
	if (keys[GLFW_KEY_LEFT_CONTROL] == true && button == GLFW_MOUSE_BUTTON_LEFT) {
		start.Type = NORMAL;
		SetColor(start);
		node.Type = START;
		SetColor(node);
		return true;
	}
	else if (keys[GLFW_KEY_LEFT_CONTROL] == true && button == GLFW_MOUSE_BUTTON_RIGHT) {
		goal.Type = NORMAL;
		SetColor(goal);
		node.Type = GOAL;
		SetColor(node);
		return true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		node.Type = BLOCK;
		SetColor(node);
		return true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {		
		node.Type = NORMAL;
		SetColor(node);
		return true;
	} else
		return false;
}

void Node::SetVisited()
{
	if (this->Type != START && this->Type != GOAL) {
		this->Type = VISITED;
		this->Color = glm::vec3(0.0f, 0.45f, 0.90f);
	}
}

bool Node::operator==(const Node & rhs) const
{
	return this->Position.x == rhs.Position.x && this->Position.y == rhs.Position.y;
}

bool Node::operator!=(const Node & rhs) const
{
	return !(this->Position.x == rhs.Position.x && this->Position.y == rhs.Position.y);
}

bool Node::operator<(const Node & rhs) const
{
	return std::tie(this->Position.x, this->Position.y) < std::tie(rhs.Position.x, rhs.Position.y);
}