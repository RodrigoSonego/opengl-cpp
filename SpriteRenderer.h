#pragma once
#include "Shader.h"
#include "Texture.h"
#include <glad/glad.h>

class SpriteRenderer
{
public:
	SpriteRenderer(Shader spriteShader);

	void RenderSprite(Texture tex, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color);

private:
	void setupRendering();

	Shader spriteShader;
	GLuint quadVAO;
};

