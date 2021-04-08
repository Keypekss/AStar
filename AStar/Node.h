#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"

class Node {
public:
	// object state
	glm::vec2 Position, Size;
	glm::vec3 Color;
	float Rotation;

	// render state
	Texture Sprite;

	// constructors
	Node();
	Node(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color = glm::vec3(1.0f));

	// draw sprite
	void DrawNode(SpriteRenderer& renderer);
};

