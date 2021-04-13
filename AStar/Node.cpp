#include "Node.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

static unsigned int count = 0;

Node::Node() 
	: Position(0.0f, 0.0f), Size(1.0f, 1.0f), Color(1.0f), Rotation(0.0f), Type(NORMAL), ID(0), IsVisited(false), Sprite()
{
	ID = ++count;
}

Node::Node(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color)
	: Position(pos), Size(size), Color(color), Rotation(0.0f), Type(NORMAL), ID(0), IsVisited(false), Sprite(sprite)
{
	ID = ++count;
	if (ID == 1)
		Type = START;
	if (ID == 10)
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