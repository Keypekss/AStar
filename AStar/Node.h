#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <tuple>

#include "Texture.h"
#include "SpriteRenderer.h"

enum NodeType {
	NORMAL,
	BLOCK,
	START,
	GOAL
};

class Node {
public:
	// object state
	glm::vec2 Position, Size;
	glm::vec3 Color;
	NodeType Type;
	float Rotation;
	unsigned int ID;
	bool IsVisited;

	// render state
	Texture Sprite;

	// constructors
	Node();
	Node(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color = glm::vec3(1.0f));
	~Node();

	// draw sprite
	void DrawNode(SpriteRenderer& renderer);
	static void ResetCount();

	// overloaded operators
	bool operator==(const Node& rhs) const;
	bool operator!=(const Node& rhs) const;
	bool operator<(const Node& rhs) const;
// 	Node& operator=(Node& rhs);

private:
};

// implement hash function so we can use Node with unordered_map
template<> struct std::hash<Node> {
	std::size_t operator()(const Node& node) const noexcept
	{
		size_t posX = std::hash<unsigned int>()(node.Position.x);
		size_t posY = std::hash<unsigned int>()(node.Position.y) << 4;
		return posX ^ posY;
		//return std::hash<unsigned int>()(node.ID ^ (node.ID << 4));		
	}
};