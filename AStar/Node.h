#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <tuple>
#include <vector>

#include "Texture.h"
#include "SpriteRenderer.h"

enum NodeType {
	NORMAL,
	BLOCK,
	START,
	GOAL,
	VISITED
};

class Node {
public:
	// object state
	glm::vec2 Position, Size;
	glm::vec3 Color;
	NodeType Type;
	float Rotation;
	unsigned int ID;

	// render state
	Texture Sprite;

	// constructors
	Node();
	Node(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color = glm::vec3(1.0f));
	~Node();

	// draw sprite
	void DrawNode(SpriteRenderer& renderer);
	
	// reset count every time the scene is rendered again
	static void ResetCount();

	// setters
	// -------
	// set color of the node based on its type
	static void SetColor(Node &node);
	// set type of the object according to input we get
	static bool SetType(Node &start, Node &goal, Node &node, int button, int action, const bool keys[]);
	void SetVisited();

	// overloaded operators
	bool operator==(const Node& rhs) const;
	bool operator!=(const Node& rhs) const;
	bool operator<(const Node& rhs) const;

private:
};

// implement hash function so we can use Node with unordered_map
template<> struct std::hash<Node> {
	std::size_t operator()(const Node& node) const noexcept
	{
		return std::hash<unsigned int>()(node.ID ^ (node.ID << 4));		
	}
};