#include "Node.h"

Node::Node() 
	: Position(0.0f, 0.0f), Size(1.0f, 1.0f), Color(1.0f), Rotation(0.0f), Sprite()
{
}

Node::Node(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color)
	: Position(pos), Size(size), Color(color), Rotation(0.0f), Sprite(sprite)
{
}

void Node::DrawNode(SpriteRenderer & renderer)
{
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}
