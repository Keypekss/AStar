#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"

class SpriteRenderer {
public:
	// Constructor (inits shaders/shapes)
	SpriteRenderer(Shader &shader);

	// destructor
	~SpriteRenderer();

	// renders a defined quad textured given sprite
	void DrawSprite(Texture &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	// Render state
	Shader shader;
	unsigned int quadVAO;

	// initialize and configures the quad's buffer and vertex attributes
	void initRenderData();
};

